/*
 * Copyright (C) 2021 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef TEE_H_
#define TEE_H_

#include <types.h>

#define TEE_GPA_MAPPING_TO_REE_MEM (64UL * 1024UL * 1024UL * 1024UL)

/* Shared memory for TEE/REE, starts from: GPA 4G - 2M, size: 2M */
#define TEE_SMC_CALL_SHARED_PAGE_GPA (4UL * 1024UL * 1024UL * 1024UL - 2UL * 1024UL * 1024UL)
#define TEE_SMC_CALL_SHARED_PAGE_SIZE (2UL * 1024UL * 1024UL)
extern uint8_t tee_smc_shared_mem[TEE_SMC_CALL_SHARED_PAGE_SIZE];

#define OPTEE_VMCALL_SMC 0x6F707400UL

bool is_tee_vm(const struct acrn_vm *vm);

#endif /* TEE_H_ */
