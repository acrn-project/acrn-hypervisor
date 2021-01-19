/*
 * Copyright (C) 2020 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * BIOS Information
 * Vendor: Intel Corporation
 * Version: TGLIFUI1.R00.3333.A04.2009091208
 * Release Date: 09/09/2020
 *
 * Base Board Information
 * Manufacturer: Intel Corporation
 * Product Name: TigerLake U DDR4 SODIMM RVP
 * Version: 1
 */

#include <board.h>
#include <vtd.h>
#include <msr.h>
#include <pci.h>
#include <misc_cfg.h>

static struct dmar_dev_scope drhd0_dev_scope[DRHD0_DEV_CNT] = {
	{
		.type   = DRHD0_DEVSCOPE0_TYPE,
		.id     = DRHD0_DEVSCOPE0_ID,
		.bus    = DRHD0_DEVSCOPE0_BUS,
		.devfun = DRHD0_DEVSCOPE0_PATH,
	},
};

static struct dmar_dev_scope drhd1_dev_scope[DRHD1_DEV_CNT] = {
	{
		.type   = DRHD1_DEVSCOPE0_TYPE,
		.id     = DRHD1_DEVSCOPE0_ID,
		.bus    = DRHD1_DEVSCOPE0_BUS,
		.devfun = DRHD1_DEVSCOPE0_PATH,
	},
};

static struct dmar_dev_scope drhd2_dev_scope[DRHD2_DEV_CNT] = {
	{
		.type   = DRHD2_DEVSCOPE0_TYPE,
		.id     = DRHD2_DEVSCOPE0_ID,
		.bus    = DRHD2_DEVSCOPE0_BUS,
		.devfun = DRHD2_DEVSCOPE0_PATH,
	},
};

static struct dmar_dev_scope drhd3_dev_scope[DRHD3_DEV_CNT] = {
	{
		.type   = DRHD3_DEVSCOPE0_TYPE,
		.id     = DRHD3_DEVSCOPE0_ID,
		.bus    = DRHD3_DEVSCOPE0_BUS,
		.devfun = DRHD3_DEVSCOPE0_PATH,
	},
};

static struct dmar_dev_scope drhd4_dev_scope[DRHD4_DEV_CNT] = {
	{
		.type   = DRHD4_DEVSCOPE0_TYPE,
		.id     = DRHD4_DEVSCOPE0_ID,
		.bus    = DRHD4_DEVSCOPE0_BUS,
		.devfun = DRHD4_DEVSCOPE0_PATH,
	},
};

static struct dmar_dev_scope drhd5_dev_scope[DRHD5_DEV_CNT] = {
	{
		.type   = DRHD5_DEVSCOPE0_TYPE,
		.id     = DRHD5_DEVSCOPE0_ID,
		.bus    = DRHD5_DEVSCOPE0_BUS,
		.devfun = DRHD5_DEVSCOPE0_PATH,
	},
	{
		.type   = DRHD5_DEVSCOPE1_TYPE,
		.id     = DRHD5_DEVSCOPE1_ID,
		.bus    = DRHD5_DEVSCOPE1_BUS,
		.devfun = DRHD5_DEVSCOPE1_PATH,
	},
};

static struct dmar_drhd drhd_info_array[DRHD_COUNT] = {
	{
		.dev_cnt       = DRHD0_DEV_CNT,
		.segment       = DRHD0_SEGMENT,
		.flags         = DRHD0_FLAGS,
		.reg_base_addr = DRHD0_REG_BASE,
		.ignore        = DRHD0_IGNORE,
		.devices       = drhd0_dev_scope
	},
	{
		.dev_cnt       = DRHD1_DEV_CNT,
		.segment       = DRHD1_SEGMENT,
		.flags         = DRHD1_FLAGS,
		.reg_base_addr = DRHD1_REG_BASE,
		.ignore        = DRHD1_IGNORE,
		.devices       = drhd1_dev_scope
	},
	{
		.dev_cnt       = DRHD2_DEV_CNT,
		.segment       = DRHD2_SEGMENT,
		.flags         = DRHD2_FLAGS,
		.reg_base_addr = DRHD2_REG_BASE,
		.ignore        = DRHD2_IGNORE,
		.devices       = drhd2_dev_scope
	},
	{
		.dev_cnt       = DRHD3_DEV_CNT,
		.segment       = DRHD3_SEGMENT,
		.flags         = DRHD3_FLAGS,
		.reg_base_addr = DRHD3_REG_BASE,
		.ignore        = DRHD3_IGNORE,
		.devices       = drhd3_dev_scope
	},
	{
		.dev_cnt       = DRHD4_DEV_CNT,
		.segment       = DRHD4_SEGMENT,
		.flags         = DRHD4_FLAGS,
		.reg_base_addr = DRHD4_REG_BASE,
		.ignore        = DRHD4_IGNORE,
		.devices       = drhd4_dev_scope
	},
	{
		.dev_cnt       = DRHD5_DEV_CNT,
		.segment       = DRHD5_SEGMENT,
		.flags         = DRHD5_FLAGS,
		.reg_base_addr = DRHD5_REG_BASE,
		.ignore        = DRHD5_IGNORE,
		.devices       = drhd5_dev_scope
	},
};

struct dmar_info plat_dmar_info = {
	.drhd_count = DRHD_COUNT,
	.drhd_units = drhd_info_array,
};

#ifdef CONFIG_RDT_ENABLED
struct platform_clos_info platform_l2_clos_array[MAX_CACHE_CLOS_NUM_ENTRIES];
struct platform_clos_info platform_l3_clos_array[MAX_CACHE_CLOS_NUM_ENTRIES];
struct platform_clos_info platform_mba_clos_array[MAX_MBA_CLOS_NUM_ENTRIES];
#endif

static const struct cpu_cx_data board_cpu_cx[3] = {
	{{SPACE_FFixedHW, 0x01U, 0x02U, 0x01U, 0x00UL}, 0x01U, 0x01U, 0x00U},	/* C1 */
	{{SPACE_FFixedHW, 0x01U, 0x02U, 0x01U, 0x31UL}, 0x02U, 0xFDU, 0x00U},	/* C2 */
	{{SPACE_FFixedHW, 0x01U, 0x02U, 0x01U, 0x60UL}, 0x03U, 0x418U, 0x00U},	/* C3 */
};

static const struct cpu_px_data board_cpu_px[12] = {
	{0x1069UL, 0x00UL, 0x0AUL, 0x0AUL, 0x002C00UL, 0x002C00UL},     /* P0 */
	{0x1068UL, 0x00UL, 0x0AUL, 0x0AUL, 0x002A00UL, 0x002A00UL},     /* P1 */
	{0xFA0UL, 0x00UL, 0x0AUL, 0x0AUL, 0x002800UL, 0x002800UL},      /* P2 */
	{0xD48UL, 0x00UL, 0x0AUL, 0x0AUL, 0x002200UL, 0x002200UL},      /* P3 */
	{0xAF0UL, 0x00UL, 0x0AUL, 0x0AUL, 0x001C00UL, 0x001C00UL},      /* P4 */
	{0xA8CUL, 0x00UL, 0x0AUL, 0x0AUL, 0x001B00UL, 0x001B00UL},      /* P5 */
	{0xA28UL, 0x00UL, 0x0AUL, 0x0AUL, 0x001A00UL, 0x001A00UL},      /* P6 */
	{0x708UL, 0x00UL, 0x0AUL, 0x0AUL, 0x001200UL, 0x001200UL},      /* P7 */
	{0x6A4UL, 0x00UL, 0x0AUL, 0x0AUL, 0x001100UL, 0x001100UL},      /* P8 */
	{0x640UL, 0x00UL, 0x0AUL, 0x0AUL, 0x001000UL, 0x001000UL},      /* P9 */
	{0x578UL, 0x00UL, 0x0AUL, 0x0AUL, 0x000E00UL, 0x000E00UL},      /* P10 */
	{0x4B0UL, 0x00UL, 0x0AUL, 0x0AUL, 0x000C00UL, 0x000C00UL},      /* P11 */
};

const struct cpu_state_table board_cpu_state_tbl = {
	"11th Gen Intel(R) Core(TM) i7-1185GRE @ 2.80GHz",
	{(uint8_t)ARRAY_SIZE(board_cpu_px), board_cpu_px,
	(uint8_t)ARRAY_SIZE(board_cpu_cx), board_cpu_cx}
};
const union pci_bdf plat_hidden_pdevs[MAX_HIDDEN_PDEVS_NUM];

#define VMSIX_ON_MSI_DEV0	.bdf.bits = {.b = 0x00U, .d = 0x1eU, .f =0x4U},
const struct vmsix_on_msi_info vmsix_on_msi_devs[MAX_VMSIX_ON_MSI_PDEVS_NUM] = {
	{VMSIX_ON_MSI_DEV0},
};
