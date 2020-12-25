/*
 * Copyright (C) 2018 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef TYPES_H
#define TYPES_H

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define __aligned(x)		__attribute__((aligned(x)))
#define __packed	__attribute__((packed))
#define	__unused	__attribute__((unused))

#ifndef ASSEMBLER

/* Define standard data types.  These definitions allow software components
 * to perform in the same manner on different target platforms.
 */
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
typedef signed long int64_t;
typedef unsigned int size_t;
typedef __builtin_va_list va_list;

typedef _Bool bool;

#ifndef NULL
#define         NULL                                ((void *) 0)
#endif

#ifndef true
#define true		((_Bool) 1)
#define false		((_Bool) 0)
#endif

#ifndef UINT64_MAX
#define UINT64_MAX	(0xffffffffffffffffUL)
#endif

#ifndef UINT32_MAX
#define UINT32_MAX	(0xffffffffU)
#endif

extern uint64_t start_tsc_1;
extern uint64_t start_tsc_2;
extern uint64_t start_tsc_3;
extern uint64_t start_tsc_3_1;
extern uint64_t start_tsc_3_2;
extern uint64_t start_tsc_3_3;
extern uint64_t start_tsc_3_4;
extern uint64_t start_tsc_3_5;
extern uint64_t start_tsc_4;
extern uint64_t start_tsc_5;

#endif /* ASSEMBLER */

#endif /* INCLUDE_TYPES_H defined */
