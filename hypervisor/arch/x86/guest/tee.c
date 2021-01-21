/*
 * Copyright (C) 2021 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <vm.h>
#include <vm_config.h>
#include <tee.h>

uint8_t tee_smc_shared_mem[TEE_SMC_CALL_SHARED_PAGE_SIZE];

bool is_tee_vm(const struct acrn_vm *vm)
{
	return (vm != NULL) && (vm->sw.kernel_type == KERNEL_TEE);
}
