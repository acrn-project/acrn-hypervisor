/*
 * Copyright (C) 2020 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef VBAR_BASE_H_
#define VBAR_BASE_H_

#define VGA_COMPATIBLE_CONTROLLER_0_VBAR              .vbar_base[0] = 0xab000000UL, \
                                                      .vbar_base[2] = 0x80000000UL

#define SIGNAL_PROCESSING_CONTROLLER_0_VBAR           .vbar_base[0] = 0xac580000UL

#define SIGNAL_PROCESSING_CONTROLLER_1_VBAR           .vbar_base[0] = 0xac5f0000UL

#define SYSTEM_PERIPHERAL_0_VBAR                      .vbar_base[0] = 0xac600000UL

#define USB_CONTROLLER_0_VBAR                         .vbar_base[0] = 0xac5c0000UL

#define USB_CONTROLLER_1_VBAR                         .vbar_base[0] = 0xac000000UL, \
                                                      .vbar_base[2] = 0xac601000UL

#define USB_CONTROLLER_2_VBAR                         .vbar_base[0] = 0xac500000UL, \
                                                      .vbar_base[2] = 0xac602000UL

#define USB_CONTROLLER_3_VBAR                         .vbar_base[0] = 0xac540000UL, \
                                                      .vbar_base[2] = 0xac603000UL

#define USB_CONTROLLER_4_VBAR                         .vbar_base[0] = 0xac5e0000UL

#define USB_CONTROLLER_5_VBAR                         .vbar_base[0] = 0xac200000UL, \
                                                      .vbar_base[2] = 0xac605000UL

#define SERIAL_BUS_CONTROLLER_0_VBAR                  .vbar_base[0] = PTDEV_HI_MMIO_START + 0x0UL

#define SERIAL_BUS_CONTROLLER_1_VBAR                  .vbar_base[0] = PTDEV_HI_MMIO_START + 0x1000UL

#define SERIAL_BUS_CONTROLLER_2_VBAR                  .vbar_base[0] = PTDEV_HI_MMIO_START + 0x2000UL

#define SERIAL_BUS_CONTROLLER_3_VBAR                  .vbar_base[0] = PTDEV_HI_MMIO_START + 0x3000UL

#define SERIAL_BUS_CONTROLLER_4_VBAR                  .vbar_base[0] = PTDEV_HI_MMIO_START + 0x4000UL

#define SERIAL_BUS_CONTROLLER_5_VBAR                  .vbar_base[0] = PTDEV_HI_MMIO_START + 0x5000UL

#define SERIAL_BUS_CONTROLLER_6_VBAR                  .vbar_base[0] = PTDEV_HI_MMIO_START + 0x6000UL

#define SERIAL_BUS_CONTROLLER_7_VBAR                  .vbar_base[0] = PTDEV_HI_MMIO_START + 0x8000UL

#define SERIAL_BUS_CONTROLLER_8_VBAR                  .vbar_base[0] = PTDEV_HI_MMIO_START + 0x9000UL

#define SERIAL_CONTROLLER_0_VBAR                      .vbar_base[0] = 0xac5d0000UL

#define RAM_MEMORY_0_VBAR                             .vbar_base[0] = 0xac5f8000UL, \
                                                      .vbar_base[2] = 0xac606000UL

#define COMMUNICATION_CONTROLLER_0_VBAR               .vbar_base[0] = 0xac60b000UL

#define COMMUNICATION_CONTROLLER_1_VBAR               .vbar_base[0] = PTDEV_HI_MMIO_START + 0x7000UL

#define SATA_CONTROLLER_0_VBAR                        .vbar_base[0] = 0xac5fe000UL, \
                                                      .vbar_base[1] = 0xac614000UL, \
                                                      .vbar_base[5] = 0xac613000UL

#define ETHERNET_CONTROLLER_0_VBAR                    .vbar_base[0] = 0xac5fc000UL, \
                                                      .vbar_base[2] = 0xac610000UL

#define ETHERNET_CONTROLLER_1_VBAR                    .vbar_base[0] = 0xac5a0000UL

#define SMBUS_0_VBAR                                  .vbar_base[0] = 0xac611000UL

#define NON_VOLATILE_MEMORY_CONTROLLER_0_VBAR         .vbar_base[0] = 0xac400000UL

#endif /* VBAR_BASE_H_ */
