/*
 * Copyright (c) 2014-2015 Sebastian Huber <sebhub@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "compat.h"
#include "test.h"

#ifdef __rtems__

#include <bsp.h>

#if defined(LIBBSP_ARM_ALTERY_CYCLONE_V_BSP_H)
  #define PLATFORM "Altera Cyclone V, Cortex-A9 MPCore"
#elif defined(LIBBSP_ARM_XILINX_ZYNQ_BSP_H)
  #define PLATFORM "Xilinx Zynq, Cortex-A9 MPCore"
#elif defined(LIBBSP_POWERPC_MPC55XXEVB_BSP_H)
  #define PLATFORM "Freescale MPC5564, e200z6"
#elif QORIQ_CHIP_VARIANT == QORIQ_CHIP_P1020
  #define PLATFORM "Freescale P1020, e500v2"
#elif QORIQ_CHIP_VARIANT == QORIQ_CHIP_T4240
  #define PLATFORM "Freescale T4240, e6500"
#elif defined(__GENMCF548X_BSP_H)
  #define PLATFORM "Freescale MCF5484, Coldfire V4e"
#elif defined(LEON3)
  #define PLATFORM "Aeroflex Gaisler NGMP, LEON4FT"
#endif

#else /* __rtems__ */

#define PLATFORM "Linux"

#endif /* __rtems__ */

static void run(void)
{
  printk(
    "<RBTestCollection platform=\"" PLATFORM "\" compiler=\"GCC %i.%i.%i\" timeUnit=\"ns\">\n",
    __GNUC__,
    __GNUC_MINOR__,
    __GNUC_PATCHLEVEL__
  );

  /* Ordered according to runtime, slowest first */
  test_rbtree_llrb();
  test_rbtree_ec();
  test_rbtree_rtems_compact();
  test_rbtree_rb_new();
  test_rbtree_rtems();
  test_rbtree_boost();
  test_rbtree_rb();
  test_rbtree_jffs2();
  test_rbtree_bsd();
  test_rbtree_bsd_for_rtems();
  test_rbtree_rb_old();
  test_rbtree_linux();
  test_rbtree_chain();

  printk("</RBTestCollection>\n");
}

#ifndef __rtems__

int main()
{
	run();

	return 0;
}

#else /* __rtems__ */

#include <rtems.h>
#include <rtems/test.h>

const char rtems_test_name[] = "TREE BENCHMARK";

static void Init(rtems_task_argument arg)
{
  rtems_test_begink();

  run();

  rtems_test_endk();
  exit(0);
}

#define CONFIGURE_MICROSECONDS_PER_TICK 1000

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_INIT_TASK_ATTRIBUTES RTEMS_FLOATING_POINT

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_UNIFIED_WORK_AREAS

#define CONFIGURE_UNLIMITED_OBJECTS

#define CONFIGURE_INIT

#include <rtems/confdefs.h>

#endif /* __rtems__ */
