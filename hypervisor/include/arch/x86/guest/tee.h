/*
 * Copyright (C) 2021 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef TEE_H_
#define TEE_H_

#include <types.h>
#include <acrn_common.h>
#include <vcpu.h>

/* Shared memory for TEE/REE, starts from: GPA 0x3FE00000, size: 2M */
#define TEE_SMC_CALL_SHARED_PAGE_GPA 0x3FE00000UL
#define TEE_SMC_CALL_SHARED_PAGE_SIZE (2UL * 1024UL * 1024UL)

/* Reserved memory for TEE */
#define TEE_SIPI_PAGE_GPA (0x80000UL)
#define TEE_SIPI_PAGE_SIZE (4UL * 1024UL)

#define TEE_GUEST_FLAG_MASK	(GUEST_FLAG_TEE)
#define REE_GUEST_FLAG_MASK	(GUEST_FLAG_REE)

#define TEE_REE_NOTIFICATION_VECTOR	0xF3U

#define TEE_SERVICE_ACCEPTED	0x0
#define TEE_SERVICE_REFUSED	0x1

bool is_tee_hypercall(uint64_t hypcall_id);
int32_t handle_tee_hypercalls(struct acrn_vcpu *vcpu, uint64_t hypcall_id);

#endif /* TEE_H_ */
