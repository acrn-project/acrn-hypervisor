/*
 * Copyright (C) 2021 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <vm.h>
#include <vm_config.h>
#include <tee.h>

bool is_tee_vm(const struct acrn_vm *vm)
{
	return (vm != NULL) && (vm->sw.kernel_type == KERNEL_TEE);
}
