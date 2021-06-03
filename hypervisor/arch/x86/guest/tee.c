/*
 * Copyright (C) 2021 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <vm.h>
#include <vm_config.h>
#include <event.h>
#include <irq.h>
#include <logmsg.h>
#include <errno.h>
#include <tee.h>

uint8_t *tee_smc_shared_mem;

static struct acrn_vm *get_tee_vm(uint64_t flags)
{
	uint16_t vm_id;
	struct acrn_vm_config *vm_config;

	for (vm_id = 0U; vm_id < CONFIG_MAX_VM_NUM; vm_id++) {
		vm_config = get_vm_config(vm_id);
		if ((vm_config->guest_flags & flags) != 0U) {
			break;
		}
	}

	return get_vm_from_vmid(vm_id);
}

static int32_t tee_boot_done(void)
{
	//struct acrn_vm_config *vm_config;
	struct acrn_vm *ree_vm;

	ree_vm = get_tee_vm(GUEST_FLAG_REE);
	//vm_config = get_vm_config(ree_vm->vm_id);
	pr_err("ree_vm->vm_id=%u\n", ree_vm->vm_id);

	while (is_created_vm(ree_vm)) {
		start_vm(ree_vm);
	}
	pr_err("tee_boot_done");
	return 0;
}

static int32_t ree_notify_tee(struct acrn_vcpu *vcpu)
{
	struct sched_event *event;
	struct acrn_vm *tee_vm;
	struct acrn_vcpu *tee_vcpu;
	uint16_t tee_cores_nr;
	int32_t ret;

	tee_cores_nr = (uint16_t)vcpu_get_gpreg(vcpu, CPU_REG_RDI);
	tee_vm = get_tee_vm(GUEST_FLAG_TEE);
	tee_vcpu = vcpu_from_vid(tee_vm, tee_cores_nr);
	event = &tee_vcpu->events[VCPU_EVENT_VIRTUAL_INTERRUPT];
	if (event->waiting_thread != NULL) {
		ret = TEE_SERVICE_ACCEPTED;
		signal_event(event);
	} else {
		ret = TEE_SERVICE_REFUSED;
	}

	return ret;
}

static int32_t tee_notify_ree(void)
{
	struct acrn_vm *ree_vm;
	struct acrn_vcpu *vcpu;

	ree_vm = get_tee_vm(GUEST_FLAG_REE);
	vcpu = vcpu_from_vid(ree_vm, BSP_CPU_ID);

	vlapic_set_intr(vcpu, TEE_REE_NOTIFICATION_VECTOR, LAPIC_TRIG_EDGE);

	return 0;
}

static int32_t get_tee_core_num(void)
{
	struct acrn_vm *tee_vm;

	tee_vm = get_tee_vm(GUEST_FLAG_TEE);
	return tee_vm->hw.created_vcpus;
}

void reserve_buffer_for_tee_shared_mem(void)
{
	uint64_t base;

	base = e820_alloc_memory(TEE_SMC_CALL_SHARED_PAGE_SIZE, ~0UL);

	tee_smc_shared_mem = (uint8_t *)base;
}

bool is_tee_hypercall(uint64_t hypcall_id)
{
	bool ret = false;

	if ((hypcall_id == HC_TEE_BOOT_DONE) || (hypcall_id == HC_NOTIFY_TEE) ||
	    (hypcall_id == HC_NOTIFY_REE) || (hypcall_id == HC_GET_TEE_CORE_NUM)) {
		ret = true;
	}

	return ret;
}

int32_t handle_tee_hypercalls(struct acrn_vcpu *vcpu, uint64_t hypcall_id)
{
	int32_t ret = -EINVAL;

	pr_err("hypercall=0x%lx %s\n", hypcall_id, __func__);

	if (hypcall_id == HC_TEE_BOOT_DONE) {
		pr_err("HC_TEE_BOOT_DONE\n");
		ret = tee_boot_done();
	} else if (hypcall_id == HC_NOTIFY_TEE) {
		pr_err("HC_NOTIFY_TEE\n");
		ret = ree_notify_tee(vcpu);
	} else if (hypcall_id == HC_NOTIFY_REE) {
		pr_err("HC_NOTIFY_REE\n");
		ret = tee_notify_ree();
	} else if (hypcall_id == HC_GET_TEE_CORE_NUM) {
		pr_err("HC_GET_TEE_CORE_NUM\n");
		ret = get_tee_core_num();
	}

	return ret;
}
