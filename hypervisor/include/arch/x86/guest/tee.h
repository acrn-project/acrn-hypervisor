/*
 * Copyright (C) 2021 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef TEE_H_
#define TEE_H_

#include <types.h>
#include <acrn_common.h>
#include <vcpu.h>\

#define TEE_GPA_MAPPING_TO_REE_MEM (64UL * 1024UL * 1024UL * 1024UL)

/* Shared memory for TEE/REE, starts from: GPA 4G - 2M, size: 2M */
#define TEE_SMC_CALL_SHARED_PAGE_GPA (4UL * 1024UL * 1024UL * 1024UL - 2UL * 1024UL * 1024UL)
#define TEE_SMC_CALL_SHARED_PAGE_SIZE (2UL * 1024UL * 1024UL)
extern uint8_t *tee_smc_shared_mem;

#define OPTEE_VMCALL_SMC 0x6F707400UL

#define TEE_GUEST_FLAG_MASK	(GUEST_FLAG_TEE)
#define REE_GUEST_FLAG_MASK	(GUEST_FLAG_REE)

#define TEE_REE_NOTIFICATION_VECTOR	0xF3U

#define TEE_SERVICE_ACCEPTED	0x0
#define TEE_SERVICE_REFUSED	0x1

void reserve_buffer_for_tee_shared_mem(void);
bool is_tee_hypercall(uint64_t hypcall_id);
int32_t handle_tee_hypercalls(struct acrn_vcpu *vcpu, uint64_t hypcall_id);

#endif /* TEE_H_ */
