/*
 * Copyright (C) 2018 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <time.h>

#include <sys/ioctl.h>

#include "dm.h"
#include "pci_core.h"
#include "virtio.h"
#include "virtio_kernel.h"
#include "vmmapi.h"			/* for vmctx */

#define VIRTIO_ECHO_RXQ		0
#define VIRTIO_ECHO_TXQ		1
#define VIRTIO_ECHO_MAXQ	2

#define VIRTIO_ECHO_RINGSZ	64

/*
 * Per-device struct
 */
struct virtio_echo {
	/* VBS-U variables */
	struct virtio_base base;
	struct virtio_vq_info queues[VIRTIO_ECHO_MAXQ];
	pthread_mutex_t mtx;
	int buf_size;
	int count;
	int cur_cnt;
	struct timespec start;
	uint64_t total_us;
	uint64_t min_us;
	uint64_t max_us;
	/* VBS-K variables */
	struct {
		enum VBS_K_STATUS status;
		int fd;
		struct vbs_dev_info dev;
		struct vbs_vqs_info vqs;
	} vbs_k;
};

static int virtio_echo_debug = 1;
#define DPRINTF(params) do { if (virtio_echo_debug) printf params; } while (0)
#define WPRINTF(params) (printf params)

static void virtio_echo_reset(void *);
static void virtio_echo_set_status(void *, uint64_t);

/* VBS-K interface functions */
static int virtio_echo_kernel_init(struct virtio_echo *);
static int virtio_echo_kernel_start(struct virtio_echo *);
static int virtio_echo_kernel_stop(struct virtio_echo *);
static int virtio_echo_kernel_reset(struct virtio_echo *);
static int virtio_echo_kernel_dev_set(struct vbs_dev_info *kdev,
				      const char *name, int vmid, int nvq,
				      uint32_t feature, uint64_t pio_start,
				      uint64_t pio_len);
static int virtio_echo_kernel_vq_set(struct vbs_vqs_info *kvqs, unsigned int nvq,
				     unsigned int idx, uint16_t qsize,
				     uint32_t pfn, uint16_t msix_idx,
				     uint64_t msix_addr, uint32_t msix_data);

static struct virtio_ops virtio_echo_ops = {
	"virtio_echo",		/* our name */
	VIRTIO_ECHO_MAXQ,	/* we support 1 virtqueue */
	0,			/* config reg size */
	virtio_echo_reset,	/* reset */
	NULL,			/* device-wide qnotify */
	NULL,			/* read virtio config */
	NULL,			/* write virtio config */
	NULL,			/* apply negotiated features */
	virtio_echo_set_status,	/* called on guest set status */
	0,			/* our capabilities */
};

/*
 * This callback gives us a chance to determine the timings
 * to kickoff VBS-K initialization
 */
static void
virtio_echo_set_status(void *base, uint64_t status)
{
	struct virtio_echo *echo;
	int nvq;
	struct msix_table_entry *mte;
	uint64_t msix_addr = 0;
	uint32_t msix_data = 0;
	int rc, i, j;

	echo = base;
	nvq = echo->base.vops->nvq;

	if (echo->vbs_k.status == VIRTIO_DEV_INIT_SUCCESS &&
	    (status & VIRTIO_CR_STATUS_DRIVER_OK)) {
		/* time to kickoff VBS-K side */
		/* init vdev first */
		rc = virtio_echo_kernel_dev_set(&echo->vbs_k.dev,
					       echo->base.vops->name,
					       echo->base.dev->vmctx->vmid,
					       nvq,
					       echo->base.negotiated_caps,
					       /*
						* currently we let VBS-K handle
						* kick register
						*/
					       echo->base.dev->bar[0].addr + 16,
					       2);

		for (i = 0; i < nvq; i++) {
			if (echo->queues[i].msix_idx != VIRTIO_MSI_NO_VECTOR) {
				j = echo->queues[i].msix_idx;
				mte = &echo->base.dev->msix.table[j];
				msix_addr = mte->addr;
				msix_data = mte->msg_data;
			}
			rc = virtio_echo_kernel_vq_set(&echo->vbs_k.vqs,
						      nvq, i,
						      echo->queues[i].qsize,
						      echo->queues[i].pfn,
						      echo->queues[i].msix_idx,
						      msix_addr,
						      msix_data);
			if (rc < 0) {
				WPRINTF(("rnd_kernel_set_vq fail,i %d ret %d\n",
					 i, rc));
				return;
			}
		}
		rc = virtio_echo_kernel_start(echo);
		if (rc < 0) {
			WPRINTF(("virtio_echo_kernel_start() failed\n"));
			echo->vbs_k.status = VIRTIO_DEV_START_FAILED;
		} else {
			echo->vbs_k.status = VIRTIO_DEV_STARTED;
		}
	}
}

/*
 * Called in virtio_echo_init(), where the initialization of the
 * PCIe device emulation is still on the way by device model.
 */
static int
virtio_echo_kernel_init(struct virtio_echo *echo)
{
	int rc;

	assert(echo->vbs_k.fd == 0);

	echo->vbs_k.fd = open("/dev/vbs_echo", O_RDWR);
	if (echo->vbs_k.fd < 0) {
		WPRINTF(("Failed to open /dev/vbs_echo!\n"));
		return -VIRTIO_ERROR_FD_OPEN_FAILED;
	}
	DPRINTF(("Open /dev/vbs_echo success!\n"));

	memset(&echo->vbs_k.dev, 0, sizeof(struct vbs_dev_info));
	memset(&echo->vbs_k.vqs, 0, sizeof(struct vbs_vqs_info));

	/* buf_size */
	rc = ioctl(echo->vbs_k.fd, VBS_K_ECHO_SET_BUFSIZE, &echo->buf_size);
	if (rc < 0) {
		WPRINTF(("VBS_K_ECHO_SET_BUFSIZE fail,ret %d\n", rc));
		return VIRTIO_ERROR_FD_OPEN_FAILED;
	}

	/* count */
	rc = ioctl(echo->vbs_k.fd, VBS_K_ECHO_SET_COUNT, &echo->count);
	if (rc < 0) {
		WPRINTF(("VBS_K_ECHO_SET_BUFSIZE fail,ret %d\n", rc));
		return VIRTIO_ERROR_FD_OPEN_FAILED;
	}

	return VIRTIO_SUCCESS;
}

static int
virtio_echo_kernel_dev_set(struct vbs_dev_info *kdev, const char *name,
			  int vmid, int nvq, uint32_t feature,
			  uint64_t pio_start, uint64_t pio_len)
{
	/* FE driver has set VIRTIO_CONFIG_S_DRIVER_OK */

	/* init kdev */
	strncpy(kdev->name, name, VBS_NAME_LEN);
	kdev->vmid = vmid;
	kdev->nvq = nvq;
	kdev->negotiated_features = feature;
	kdev->pio_range_start = pio_start;
	kdev->pio_range_len = pio_len;

	return VIRTIO_SUCCESS;
}

static int
virtio_echo_kernel_vq_set(struct vbs_vqs_info *kvqs, unsigned int nvq,
			 unsigned int idx, uint16_t qsize, uint32_t pfn,
			 uint16_t msix_idx, uint64_t msix_addr,
			 uint32_t msix_data)
{
	/* FE driver has set VIRTIO_CONFIG_S_DRIVER_OK */
	if (nvq <= idx) {
		WPRINTF(("%s: wrong idx!\n", __func__));
		return -VIRTIO_ERROR_GENERAL;
	}

	/* init kvqs */
	kvqs->nvq = nvq;
	kvqs->vqs[idx].qsize = qsize;
	kvqs->vqs[idx].pfn = pfn;
	kvqs->vqs[idx].msix_idx = msix_idx;
	kvqs->vqs[idx].msix_addr = msix_addr;
	kvqs->vqs[idx].msix_data = msix_data;

	return VIRTIO_SUCCESS;
}

static int
virtio_echo_kernel_start(struct virtio_echo *echo)
{
	if (vbs_kernel_start(echo->vbs_k.fd,
			     &echo->vbs_k.dev,
			     &echo->vbs_k.vqs) < 0) {
		WPRINTF(("Failed in vbs_k_start!\n"));
		return -VIRTIO_ERROR_START;
	}

	WPRINTF(("virtio_echo_kernel_start!\n"));
	return VIRTIO_SUCCESS;
}

static int
virtio_echo_kernel_stop(struct virtio_echo *echo)
{
	/* device specific cleanups here */
	return vbs_kernel_stop(echo->vbs_k.fd);
}

static int
virtio_echo_kernel_reset(struct virtio_echo *echo)
{
	memset(&echo->vbs_k.dev, 0, sizeof(struct vbs_dev_info));
	memset(&echo->vbs_k.vqs, 0, sizeof(struct vbs_vqs_info));

	return vbs_kernel_reset(echo->vbs_k.fd);
}

static void
virtio_echo_reset(void *base)
{
	struct virtio_echo *echo;

	echo = base;

	DPRINTF(("virtio_echo: device reset requested !\n"));
	virtio_reset_dev(&echo->base);
	DPRINTF(("virtio_echo: kstatus %d\n", echo->vbs_k.status));
	if (echo->vbs_k.status == VIRTIO_DEV_STARTED) {
		DPRINTF(("virtio_echo: VBS-K reset requested!\n"));
		virtio_echo_kernel_stop(echo);
		virtio_echo_kernel_reset(echo);
		echo->vbs_k.status = VIRTIO_DEV_INIT_SUCCESS;
	}
}

/* send data to guest */
static void
virtio_echo_notify_rx_vq(void *vdev, struct virtio_vq_info *vq)
{
	struct virtio_echo *echo;
	struct iovec iov;
	int n, size;
	uint16_t idx;

	echo = vdev;

	if (echo->cur_cnt < echo->count) {
		if (clock_gettime(CLOCK_MONOTONIC, &echo->start))
			WPRINTF(("%s, clock_gettime error\n", __func__));
		if (vq_has_descs(vq)) {
			n = vq_getchain(vq, &idx, &iov, 1, NULL);
			assert(n == 1);
			size = echo->buf_size <= iov.iov_len
				? echo->buf_size
				: iov.iov_len;
			memset(iov.iov_base, 0xAA, size);
			vq_relchain(vq, idx, size);
			echo->cur_cnt++;
		}
		vq_endchains(vq, 1);
	} else {
		WPRINTF(("%s, total_us = %ld, count = %d\n",
			__func__, echo->total_us, echo->cur_cnt));
		WPRINTF(("%s, min_us = %ld\n",
			__func__, echo->min_us));
		WPRINTF(("%s, max_us = %ld\n",
			__func__, echo->max_us));
		WPRINTF(("%s, average_us = %ld\n",
			__func__, echo->total_us/echo->cur_cnt));
	}
}

/* receive data from guest */
static void
virtio_echo_notify_tx_vq(void *vdev, struct virtio_vq_info *vq)
{
	struct virtio_echo *echo;
	struct iovec iov;
	int n;
	uint16_t idx;
	struct timespec end;
	uint64_t this_us;

	echo = vdev;

	if (vq_has_descs(vq)) {
		n = vq_getchain(vq, &idx, &iov, 1, NULL);
		assert(n == 1);

		/* time calculation */
		if (clock_gettime(CLOCK_MONOTONIC, &end))
			WPRINTF(("%s, clock_gettime error\n", __func__));
		this_us = ((end.tv_sec * 1000 * 1000) + (end.tv_nsec / 1000)) -
			((echo->start.tv_sec * 1000 * 1000) + (echo->start.tv_nsec / 1000));
		if (this_us < echo->min_us)
			echo->min_us = this_us;
		if (this_us > echo->max_us)
			echo->max_us = this_us;

		vq_relchain(vq, idx, iov.iov_len); /* Release the chain */
	}
	vq_endchains(vq, 1);	/* Generate interrupt if appropriate. */
}

static int
virtio_echo_init(struct vmctx *ctx, struct pci_vdev *dev, char *opts)
{
	struct virtio_echo *echo = NULL;
	pthread_mutexattr_t attr;
	int rc;
	char *opt;
	char *vbs_k_opt = NULL;
	int buf_size = 0;
	int count = 0;
	enum VBS_K_STATUS kstat = VIRTIO_DEV_INITIAL;

	while ((opt = strsep(&opts, ",")) != NULL) {
		/* vbs_k_opt should be kernel=on */
		vbs_k_opt = strsep(&opt, "=");
		if (!vbs_k_opt) {
			WPRINTF(("virtio_echo: invalid command line"));
			goto fail;
		}
		DPRINTF(("vbs_k_opt is %s\n", vbs_k_opt));
		if (strncmp(vbs_k_opt, "kernel", 6) == 0) {
			if (opt != NULL) {
				if (strncmp(opt, "on", 2) == 0) {
					kstat = VIRTIO_DEV_PRE_INIT;
					WPRINTF(("virtio_echo: VBS-K initializing..."));
				}
			}
		} else if (strncmp(vbs_k_opt, "buf_size", 8) == 0) {
			if (opt != NULL)
				buf_size = atoi(opt);
		} else if (strncmp(vbs_k_opt, "count", 5) == 0) {
			if (opt != NULL)
				count = atoi(opt);
		}
	}

	echo = calloc(1, sizeof(struct virtio_echo));
	if (!echo) {
		WPRINTF(("virtio_echo: calloc returns NULL\n"));
		goto fail;
	}

	if (count <= 0)
		count = 100;
	if (buf_size <= 0)
		buf_size = 32 * 1024;

	echo->buf_size = buf_size;
	echo->count = count;
	echo->vbs_k.status = kstat;
	echo->total_us = 0;
	echo->min_us = ULONG_MAX;
	echo->max_us = 0;

	/* init mutex attribute properly */
	rc = pthread_mutexattr_init(&attr);
	if (rc)
		DPRINTF(("mutexattr init failed with erro %d!\n", rc));
	if (virtio_uses_msix()) {
		rc = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_DEFAULT);
		if (rc)
			DPRINTF(("virtio_msix: mutexattr_settype failed with "
				"error %d!\n", rc));
	} else {
		rc = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		if (rc)
			DPRINTF(("virtio_intx: mutexattr_settype failed with "
				"error %d!\n", rc));
	}
	rc = pthread_mutex_init(&echo->mtx, &attr);
	if (rc)
		DPRINTF(("mutex init failed with error %d!\n", rc));

	virtio_linkup(&echo->base, &virtio_echo_ops, echo, dev, echo->queues);
	echo->base.mtx = &echo->mtx;

	echo->queues[VIRTIO_ECHO_RXQ].qsize = VIRTIO_ECHO_RINGSZ;
	echo->queues[VIRTIO_ECHO_RXQ].notify = virtio_echo_notify_rx_vq;

	echo->queues[VIRTIO_ECHO_TXQ].qsize = VIRTIO_ECHO_RINGSZ;
	echo->queues[VIRTIO_ECHO_TXQ].notify = virtio_echo_notify_tx_vq;

	if (echo->vbs_k.status == VIRTIO_DEV_PRE_INIT) {
		DPRINTF(("%s: VBS-K option detected!\n", __func__));
		rc = virtio_echo_kernel_init(echo);
		if (rc < 0) {
			WPRINTF(("virtio_echo: VBS-K init failed,error %d!\n",
				 rc));
			echo->vbs_k.status = VIRTIO_DEV_INIT_FAILED;
		} else {
			echo->vbs_k.status = VIRTIO_DEV_INIT_SUCCESS;
		}
	}

	/* initialize config space */
	pci_set_cfgdata16(dev, PCIR_DEVICE, VIRTIO_DEV_RANDOM);
	pci_set_cfgdata16(dev, PCIR_VENDOR, VIRTIO_VENDOR);
	pci_set_cfgdata8(dev, PCIR_CLASS, PCIC_CRYPTO);
	pci_set_cfgdata16(dev, PCIR_SUBDEV_0, 21);
	pci_set_cfgdata16(dev, PCIR_SUBVEND_0, VIRTIO_VENDOR);

	if (virtio_interrupt_init(&echo->base, virtio_uses_msix())) {
		goto fail;
	}

	virtio_set_io_bar(&echo->base, 0);

	return 0;

fail:
	if (echo) {
		if (echo->vbs_k.status == VIRTIO_DEV_INIT_SUCCESS) {
			close(echo->vbs_k.fd);
		}
		free(echo);
	}
	return -1;
}

static void
virtio_echo_deinit(struct vmctx *ctx, struct pci_vdev *dev, char *opts)
{
	struct virtio_echo *echo;

	echo = dev->arg;
	if (echo == NULL) {
		DPRINTF(("%s: echo is NULL\n", __func__));
		return;
	}

	if (echo->vbs_k.status == VIRTIO_DEV_STARTED) {
		DPRINTF(("%s: deinit virtio_echo_k!\n", __func__));
		virtio_echo_kernel_stop(echo);
		virtio_echo_kernel_reset(echo);
		echo->vbs_k.status = VIRTIO_DEV_INITIAL;
		assert(echo->vbs_k.fd >= 0);
		close(echo->vbs_k.fd);
		echo->vbs_k.fd = -1;
	}

	DPRINTF(("%s: free struct virtio_echo!\n", __func__));
	free(echo);
}

struct pci_vdev_ops pci_ops_virtio_echo = {
	.class_name	= "virtio-echo",
	.vdev_init	= virtio_echo_init,
	.vdev_deinit	= virtio_echo_deinit,
	.vdev_barwrite	= virtio_pci_write,
	.vdev_barread	= virtio_pci_read
};
DEFINE_PCI_DEVTYPE(pci_ops_virtio_echo);
