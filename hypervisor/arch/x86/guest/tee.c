/*
 * Copyright (C) 2021 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <vm.h>
#include <vm_config.h>
#include <event.h>
#include <irq.h>
#include <tee.h>

uint8_t *tee_smc_shared_mem;

bool is_tee_vm(const struct acrn_vm *vm)
{
	return (vm != NULL) && (vm->sw.kernel_type == KERNEL_TEE);
}

void suspend_ree_vm(void)
{
	struct acrn_vm *ree_vm;
	struct acrn_vcpu *vcpu = NULL;
	uint16_t i;

	ree_vm = get_ree_vm();

	foreach_vcpu(i, ree_vm, vcpu) {
		vcpu_make_request(vcpu, ACRN_REQUEST_REE_WAIT);
	}
}

void resume_ree_vm(void)
{
	struct acrn_vm *ree_vm;
	struct acrn_vcpu *vcpu = NULL;
	uint16_t i;

	ree_vm = get_ree_vm();

	foreach_vcpu(i, ree_vm, vcpu) {
		signal_event(&vcpu->events[VCPU_EVENT_REE_WAIT]);
	}
}

int32_t tee_service_done(void)
{
	struct acrn_vm *ree_vm;
	struct acrn_vcpu *vcpu;

	ree_vm = get_ree_vm();
	vcpu = vcpu_from_vid(ree_vm, BSP_CPU_ID);

	vlapic_set_intr(vcpu, TEE_REE_NOTIFICATION_VECTOR, LAPIC_TRIG_EDGE);

	return 0;
}

void reserve_buffer_for_tee_shared_mem(void)
{
	uint64_t base;

	base = e820_alloc_memory(TEE_SMC_CALL_SHARED_PAGE_SIZE, ~0UL);

	tee_smc_shared_mem = (uint8_t *)base;
}
