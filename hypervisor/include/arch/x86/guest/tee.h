/*
 * Copyright (C) 2021 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef TEE_H_
#define TEE_H_

/* Shared memory for TEE/REE, starts from: GPA 4G - 2M, size: 2M */
#define TEE_SMC_CALL_SHARED_PAGE_GPA (4UL * 1024UL * 1024UL * 1024UL - 2UL * 1024UL * 1024UL)
#define TEE_SMC_CALL_SHARED_PAGE_SIZE (2UL * 1024UL * 1024UL)
#define TEE_GUEST_FLAG_MASK	(GUEST_FLAG_TEE)
#define REE_GUEST_FLAG_MASK	(GUEST_FLAG_REE)

#endif /* TEE_H_ */
