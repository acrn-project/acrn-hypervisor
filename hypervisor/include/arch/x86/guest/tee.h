/*
 * Copyright (C) 2021 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef TEE_H_
#define TEE_H_

#define TEE_GPA_MAPPING_TO_REE_MEM (64UL * 1024UL * 1024UL * 1024UL)

#define OPTEE_VMCALL_SMC 0x6F707400UL

bool is_tee_vm(const struct acrn_vm *vm);

#endif /* TEE_H_ */
