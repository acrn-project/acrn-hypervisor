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

#ifndef PCI_DEVICES_H_
#define PCI_DEVICES_H_

#define HOST_BRIDGE                             .pbdf.bits = {.b = 0x00U, .d = 0x00U, .f = 0x00U}

#define HOST_BRIDGE_1                           .pbdf.bits = {.b = 0x00U, .d = 0x10U, .f = 0x05U}

#define VGA_COMPATIBLE_CONTROLLER_0             .pbdf.bits = {.b = 0x00U, .d = 0x02U, .f = 0x00U}

#define SIGNAL_PROCESSING_CONTROLLER_0          .pbdf.bits = {.b = 0x00U, .d = 0x04U, .f = 0x00U}

#define SIGNAL_PROCESSING_CONTROLLER_1          .pbdf.bits = {.b = 0x00U, .d = 0x0AU, .f = 0x00U}

#define PCI_BRIDGE_0                            .pbdf.bits = {.b = 0x00U, .d = 0x06U, .f = 0x00U}

#define PCI_BRIDGE_1                            .pbdf.bits = {.b = 0x00U, .d = 0x07U, .f = 0x00U}

#define PCI_BRIDGE_2                            .pbdf.bits = {.b = 0x00U, .d = 0x07U, .f = 0x01U}

#define PCI_BRIDGE_3                            .pbdf.bits = {.b = 0x00U, .d = 0x07U, .f = 0x02U}

#define PCI_BRIDGE_4                            .pbdf.bits = {.b = 0x00U, .d = 0x07U, .f = 0x03U}

#define SYSTEM_PERIPHERAL_0                     .pbdf.bits = {.b = 0x00U, .d = 0x08U, .f = 0x00U}

#define USB_CONTROLLER_0                        .pbdf.bits = {.b = 0x00U, .d = 0x0DU, .f = 0x00U}

#define USB_CONTROLLER_1                        .pbdf.bits = {.b = 0x00U, .d = 0x0DU, .f = 0x01U}

#define USB_CONTROLLER_2                        .pbdf.bits = {.b = 0x00U, .d = 0x0DU, .f = 0x02U}

#define USB_CONTROLLER_3                        .pbdf.bits = {.b = 0x00U, .d = 0x0DU, .f = 0x03U}

#define USB_CONTROLLER_4                        .pbdf.bits = {.b = 0x00U, .d = 0x14U, .f = 0x00U}

#define USB_CONTROLLER_5                        .pbdf.bits = {.b = 0x00U, .d = 0x14U, .f = 0x01U}

#define SERIAL_BUS_CONTROLLER_0                 .pbdf.bits = {.b = 0x00U, .d = 0x10U, .f = 0x00U}

#define SERIAL_BUS_CONTROLLER_1                 .pbdf.bits = {.b = 0x00U, .d = 0x15U, .f = 0x00U}

#define SERIAL_BUS_CONTROLLER_2                 .pbdf.bits = {.b = 0x00U, .d = 0x15U, .f = 0x01U}

#define SERIAL_BUS_CONTROLLER_3                 .pbdf.bits = {.b = 0x00U, .d = 0x15U, .f = 0x02U}

#define SERIAL_BUS_CONTROLLER_4                 .pbdf.bits = {.b = 0x00U, .d = 0x15U, .f = 0x03U}

#define SERIAL_BUS_CONTROLLER_5                 .pbdf.bits = {.b = 0x00U, .d = 0x19U, .f = 0x00U}

#define SERIAL_BUS_CONTROLLER_6                 .pbdf.bits = {.b = 0x00U, .d = 0x19U, .f = 0x01U}

#define SERIAL_BUS_CONTROLLER_7                 .pbdf.bits = {.b = 0x00U, .d = 0x1EU, .f = 0x03U}

#define SERIAL_BUS_CONTROLLER_8                 .pbdf.bits = {.b = 0x00U, .d = 0x1FU, .f = 0x05U}

#define SERIAL_CONTROLLER_0                     .pbdf.bits = {.b = 0x00U, .d = 0x12U, .f = 0x00U}

#define RAM_MEMORY_0                            .pbdf.bits = {.b = 0x00U, .d = 0x14U, .f = 0x02U}

#define COMMUNICATION_CONTROLLER_0              .pbdf.bits = {.b = 0x00U, .d = 0x16U, .f = 0x00U}

#define COMMUNICATION_CONTROLLER_1              .pbdf.bits = {.b = 0x00U, .d = 0x1EU, .f = 0x00U}

#define SATA_CONTROLLER_0                       .pbdf.bits = {.b = 0x00U, .d = 0x17U, .f = 0x00U}

#define ETHERNET_CONTROLLER_0                   .pbdf.bits = {.b = 0x00U, .d = 0x1EU, .f = 0x04U}

#define ETHERNET_CONTROLLER_1                   .pbdf.bits = {.b = 0x00U, .d = 0x1FU, .f = 0x06U}

#define ISA_BRIDGE_0                            .pbdf.bits = {.b = 0x00U, .d = 0x1FU, .f = 0x00U}

#define SMBUS_0                                 .pbdf.bits = {.b = 0x00U, .d = 0x1FU, .f = 0x04U}

#define NON_VOLATILE_MEMORY_CONTROLLER_0        .pbdf.bits = {.b = 0x01U, .d = 0x00U, .f = 0x00U}

#endif /* PCI_DEVICES_H_ */
