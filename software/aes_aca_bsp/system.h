/*
 * system.h - SOPC Builder system and BSP software package information
 *
 * Machine generated for CPU 'nios2_qsys_0' in SOPC Builder design 'qsys_top'
 * SOPC Builder design path: ../../qsys_top.sopcinfo
 *
 * Generated: Tue May 23 02:29:20 CEST 2017
 */

/*
 * DO NOT MODIFY THIS FILE
 *
 * Changing this file will have subtle consequences
 * which will almost certainly lead to a nonfunctioning
 * system. If you do modify this file, be aware that your
 * changes will be overwritten and lost when this file
 * is generated again.
 *
 * DO NOT MODIFY THIS FILE
 */

/*
 * License Agreement
 *
 * Copyright (c) 2008
 * Altera Corporation, San Jose, California, USA.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This agreement shall be governed in all respects by the laws of the State
 * of California and by the laws of the United States of America.
 */

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

/* Include definitions from linker script generator */
#include "linker.h"


/*
 * CPU configuration
 *
 */

#define ALT_CPU_ARCHITECTURE "altera_nios2_gen2"
#define ALT_CPU_BIG_ENDIAN 0
#define ALT_CPU_BREAK_ADDR 0x00021820
#define ALT_CPU_CPU_ARCH_NIOS2_R1
#define ALT_CPU_CPU_FREQ 50000000u
#define ALT_CPU_CPU_ID_SIZE 1
#define ALT_CPU_CPU_ID_VALUE 0x00000000
#define ALT_CPU_CPU_IMPLEMENTATION "fast"
#define ALT_CPU_DATA_ADDR_WIDTH 0x12
#define ALT_CPU_DCACHE_LINE_SIZE 0
#define ALT_CPU_DCACHE_LINE_SIZE_LOG2 0
#define ALT_CPU_DCACHE_SIZE 0
#define ALT_CPU_EXCEPTION_ADDR 0x00010020
#define ALT_CPU_FLASH_ACCELERATOR_LINES 0
#define ALT_CPU_FLASH_ACCELERATOR_LINE_SIZE 0
#define ALT_CPU_FLUSHDA_SUPPORTED
#define ALT_CPU_FREQ 50000000
#define ALT_CPU_HARDWARE_DIVIDE_PRESENT 0
#define ALT_CPU_HARDWARE_MULTIPLY_PRESENT 1
#define ALT_CPU_HARDWARE_MULX_PRESENT 0
#define ALT_CPU_HAS_DEBUG_CORE 1
#define ALT_CPU_HAS_DEBUG_STUB
#define ALT_CPU_HAS_EXTRA_EXCEPTION_INFO
#define ALT_CPU_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define ALT_CPU_HAS_JMPI_INSTRUCTION
#define ALT_CPU_ICACHE_LINE_SIZE 32
#define ALT_CPU_ICACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_ICACHE_SIZE 4096
#define ALT_CPU_INST_ADDR_WIDTH 0x12
#define ALT_CPU_NAME "nios2_qsys_0"
#define ALT_CPU_NUM_OF_SHADOW_REG_SETS 0
#define ALT_CPU_OCI_VERSION 1
#define ALT_CPU_RESET_ADDR 0x00021000


/*
 * CPU configuration (with legacy prefix - don't use these anymore)
 *
 */

#define NIOS2_BIG_ENDIAN 0
#define NIOS2_BREAK_ADDR 0x00021820
#define NIOS2_CPU_ARCH_NIOS2_R1
#define NIOS2_CPU_FREQ 50000000u
#define NIOS2_CPU_ID_SIZE 1
#define NIOS2_CPU_ID_VALUE 0x00000000
#define NIOS2_CPU_IMPLEMENTATION "fast"
#define NIOS2_DATA_ADDR_WIDTH 0x12
#define NIOS2_DCACHE_LINE_SIZE 0
#define NIOS2_DCACHE_LINE_SIZE_LOG2 0
#define NIOS2_DCACHE_SIZE 0
#define NIOS2_EXCEPTION_ADDR 0x00010020
#define NIOS2_FLASH_ACCELERATOR_LINES 0
#define NIOS2_FLASH_ACCELERATOR_LINE_SIZE 0
#define NIOS2_FLUSHDA_SUPPORTED
#define NIOS2_HARDWARE_DIVIDE_PRESENT 0
#define NIOS2_HARDWARE_MULTIPLY_PRESENT 1
#define NIOS2_HARDWARE_MULX_PRESENT 0
#define NIOS2_HAS_DEBUG_CORE 1
#define NIOS2_HAS_DEBUG_STUB
#define NIOS2_HAS_EXTRA_EXCEPTION_INFO
#define NIOS2_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define NIOS2_HAS_JMPI_INSTRUCTION
#define NIOS2_ICACHE_LINE_SIZE 32
#define NIOS2_ICACHE_LINE_SIZE_LOG2 5
#define NIOS2_ICACHE_SIZE 4096
#define NIOS2_INST_ADDR_WIDTH 0x12
#define NIOS2_NUM_OF_SHADOW_REG_SETS 0
#define NIOS2_OCI_VERSION 1
#define NIOS2_RESET_ADDR 0x00021000


/*
 * Define for each module class mastered by the CPU
 *
 */

#define __AES_ACCELERATOR
#define __ALTERA_AVALON_EPCS_FLASH_CONTROLLER
#define __ALTERA_AVALON_JTAG_UART
#define __ALTERA_AVALON_ONCHIP_MEMORY2
#define __ALTERA_AVALON_PERFORMANCE_COUNTER
#define __ALTERA_AVALON_PIO
#define __ALTERA_AVALON_SYSID_QSYS
#define __ALTERA_AVALON_TIMER
#define __ALTERA_NIOS2_GEN2
#define __FULLRGB_96LED


/*
 * System configuration
 *
 */

#define ALT_DEVICE_FAMILY "Cyclone IV E"
#define ALT_ENHANCED_INTERRUPT_API_PRESENT
#define ALT_IRQ_BASE NULL
#define ALT_LOG_PORT "/dev/null"
#define ALT_LOG_PORT_BASE 0x0
#define ALT_LOG_PORT_DEV null
#define ALT_LOG_PORT_TYPE ""
#define ALT_NUM_EXTERNAL_INTERRUPT_CONTROLLERS 0
#define ALT_NUM_INTERNAL_INTERRUPT_CONTROLLERS 1
#define ALT_NUM_INTERRUPT_CONTROLLERS 1
#define ALT_STDERR "/dev/jtag_uart_0"
#define ALT_STDERR_BASE 0x222e8
#define ALT_STDERR_DEV jtag_uart_0
#define ALT_STDERR_IS_JTAG_UART
#define ALT_STDERR_PRESENT
#define ALT_STDERR_TYPE "altera_avalon_jtag_uart"
#define ALT_STDIN "/dev/jtag_uart_0"
#define ALT_STDIN_BASE 0x222e8
#define ALT_STDIN_DEV jtag_uart_0
#define ALT_STDIN_IS_JTAG_UART
#define ALT_STDIN_PRESENT
#define ALT_STDIN_TYPE "altera_avalon_jtag_uart"
#define ALT_STDOUT "/dev/jtag_uart_0"
#define ALT_STDOUT_BASE 0x222e8
#define ALT_STDOUT_DEV jtag_uart_0
#define ALT_STDOUT_IS_JTAG_UART
#define ALT_STDOUT_PRESENT
#define ALT_STDOUT_TYPE "altera_avalon_jtag_uart"
#define ALT_SYSTEM_NAME "qsys_top"


/*
 * aes_accelerator_0 configuration
 *
 */

#define AES_ACCELERATOR_0_BASE 0x22220
#define AES_ACCELERATOR_0_IRQ -1
#define AES_ACCELERATOR_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define AES_ACCELERATOR_0_NAME "/dev/aes_accelerator_0"
#define AES_ACCELERATOR_0_SPAN 32
#define AES_ACCELERATOR_0_TYPE "aes_accelerator"
#define ALT_MODULE_CLASS_aes_accelerator_0 aes_accelerator


/*
 * buttons configuration
 *
 */

#define ALT_MODULE_CLASS_buttons altera_avalon_pio
#define BUTTONS_BASE 0x222d0
#define BUTTONS_BIT_CLEARING_EDGE_REGISTER 1
#define BUTTONS_BIT_MODIFYING_OUTPUT_REGISTER 0
#define BUTTONS_CAPTURE 1
#define BUTTONS_DATA_WIDTH 4
#define BUTTONS_DO_TEST_BENCH_WIRING 0
#define BUTTONS_DRIVEN_SIM_VALUE 0
#define BUTTONS_EDGE_TYPE "FALLING"
#define BUTTONS_FREQ 50000000
#define BUTTONS_HAS_IN 1
#define BUTTONS_HAS_OUT 1
#define BUTTONS_HAS_TRI 0
#define BUTTONS_IRQ 3
#define BUTTONS_IRQ_INTERRUPT_CONTROLLER_ID 0
#define BUTTONS_IRQ_TYPE "EDGE"
#define BUTTONS_NAME "/dev/buttons"
#define BUTTONS_RESET_VALUE 0
#define BUTTONS_SPAN 16
#define BUTTONS_TYPE "altera_avalon_pio"


/*
 * counter_0 configuration
 *
 */

#define ALT_MODULE_CLASS_counter_0 altera_avalon_performance_counter
#define COUNTER_0_BASE 0x22280
#define COUNTER_0_HOW_MANY_SECTIONS 1
#define COUNTER_0_IRQ -1
#define COUNTER_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define COUNTER_0_NAME "/dev/counter_0"
#define COUNTER_0_SPAN 32
#define COUNTER_0_TYPE "altera_avalon_performance_counter"


/*
 * epcs configuration
 *
 */

#define ALT_MODULE_CLASS_epcs altera_avalon_epcs_flash_controller
#define EPCS_BASE 0x21000
#define EPCS_IRQ 5
#define EPCS_IRQ_INTERRUPT_CONTROLLER_ID 0
#define EPCS_NAME "/dev/epcs"
#define EPCS_REGISTER_OFFSET 1024
#define EPCS_SPAN 2048
#define EPCS_TYPE "altera_avalon_epcs_flash_controller"


/*
 * fullrgb_96led_0 configuration
 *
 */

#define ALT_MODULE_CLASS_fullrgb_96led_0 fullrgb_96led
#define FULLRGB_96LED_0_BASE 0x22000
#define FULLRGB_96LED_0_IRQ -1
#define FULLRGB_96LED_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define FULLRGB_96LED_0_NAME "/dev/fullrgb_96led_0"
#define FULLRGB_96LED_0_SPAN 512
#define FULLRGB_96LED_0_TYPE "fullrgb_96led"


/*
 * hal configuration
 *
 */

#define ALT_INCLUDE_INSTRUCTION_RELATED_EXCEPTION_API
#define ALT_MAX_FD 4
#define ALT_SYS_CLK none
#define ALT_TIMESTAMP_CLK none


/*
 * joystick configuration
 *
 */

#define ALT_MODULE_CLASS_joystick altera_avalon_pio
#define JOYSTICK_BASE 0x222a0
#define JOYSTICK_BIT_CLEARING_EDGE_REGISTER 1
#define JOYSTICK_BIT_MODIFYING_OUTPUT_REGISTER 0
#define JOYSTICK_CAPTURE 1
#define JOYSTICK_DATA_WIDTH 5
#define JOYSTICK_DO_TEST_BENCH_WIRING 0
#define JOYSTICK_DRIVEN_SIM_VALUE 0
#define JOYSTICK_EDGE_TYPE "FALLING"
#define JOYSTICK_FREQ 50000000
#define JOYSTICK_HAS_IN 1
#define JOYSTICK_HAS_OUT 0
#define JOYSTICK_HAS_TRI 0
#define JOYSTICK_IRQ 4
#define JOYSTICK_IRQ_INTERRUPT_CONTROLLER_ID 0
#define JOYSTICK_IRQ_TYPE "LEVEL"
#define JOYSTICK_NAME "/dev/joystick"
#define JOYSTICK_RESET_VALUE 0
#define JOYSTICK_SPAN 16
#define JOYSTICK_TYPE "altera_avalon_pio"


/*
 * jtag_uart_0 configuration
 *
 */

#define ALT_MODULE_CLASS_jtag_uart_0 altera_avalon_jtag_uart
#define JTAG_UART_0_BASE 0x222e8
#define JTAG_UART_0_IRQ 2
#define JTAG_UART_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define JTAG_UART_0_NAME "/dev/jtag_uart_0"
#define JTAG_UART_0_READ_DEPTH 256
#define JTAG_UART_0_READ_THRESHOLD 8
#define JTAG_UART_0_SPAN 8
#define JTAG_UART_0_TYPE "altera_avalon_jtag_uart"
#define JTAG_UART_0_WRITE_DEPTH 256
#define JTAG_UART_0_WRITE_THRESHOLD 8


/*
 * onchip_memory configuration
 *
 */

#define ALT_MODULE_CLASS_onchip_memory altera_avalon_onchip_memory2
#define ONCHIP_MEMORY_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define ONCHIP_MEMORY_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define ONCHIP_MEMORY_BASE 0x10000
#define ONCHIP_MEMORY_CONTENTS_INFO ""
#define ONCHIP_MEMORY_DUAL_PORT 0
#define ONCHIP_MEMORY_GUI_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEMORY_INIT_CONTENTS_FILE "qsys_top_onchip_memory"
#define ONCHIP_MEMORY_INIT_MEM_CONTENT 1
#define ONCHIP_MEMORY_INSTANCE_ID "NONE"
#define ONCHIP_MEMORY_IRQ -1
#define ONCHIP_MEMORY_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ONCHIP_MEMORY_NAME "/dev/onchip_memory"
#define ONCHIP_MEMORY_NON_DEFAULT_INIT_FILE_ENABLED 0
#define ONCHIP_MEMORY_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEMORY_READ_DURING_WRITE_MODE "DONT_CARE"
#define ONCHIP_MEMORY_SINGLE_CLOCK_OP 0
#define ONCHIP_MEMORY_SIZE_MULTIPLE 1
#define ONCHIP_MEMORY_SIZE_VALUE 49152
#define ONCHIP_MEMORY_SPAN 49152
#define ONCHIP_MEMORY_TYPE "altera_avalon_onchip_memory2"
#define ONCHIP_MEMORY_WRITABLE 1


/*
 * switch0 configuration
 *
 */

#define ALT_MODULE_CLASS_switch0 altera_avalon_pio
#define SWITCH0_BASE 0x222c0
#define SWITCH0_BIT_CLEARING_EDGE_REGISTER 0
#define SWITCH0_BIT_MODIFYING_OUTPUT_REGISTER 0
#define SWITCH0_CAPTURE 0
#define SWITCH0_DATA_WIDTH 8
#define SWITCH0_DO_TEST_BENCH_WIRING 0
#define SWITCH0_DRIVEN_SIM_VALUE 0
#define SWITCH0_EDGE_TYPE "NONE"
#define SWITCH0_FREQ 50000000
#define SWITCH0_HAS_IN 0
#define SWITCH0_HAS_OUT 0
#define SWITCH0_HAS_TRI 1
#define SWITCH0_IRQ -1
#define SWITCH0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SWITCH0_IRQ_TYPE "NONE"
#define SWITCH0_NAME "/dev/switch0"
#define SWITCH0_RESET_VALUE 0
#define SWITCH0_SPAN 16
#define SWITCH0_TYPE "altera_avalon_pio"


/*
 * switch1 configuration
 *
 */

#define ALT_MODULE_CLASS_switch1 altera_avalon_pio
#define SWITCH1_BASE 0x222b0
#define SWITCH1_BIT_CLEARING_EDGE_REGISTER 0
#define SWITCH1_BIT_MODIFYING_OUTPUT_REGISTER 0
#define SWITCH1_CAPTURE 0
#define SWITCH1_DATA_WIDTH 8
#define SWITCH1_DO_TEST_BENCH_WIRING 0
#define SWITCH1_DRIVEN_SIM_VALUE 0
#define SWITCH1_EDGE_TYPE "NONE"
#define SWITCH1_FREQ 50000000
#define SWITCH1_HAS_IN 0
#define SWITCH1_HAS_OUT 0
#define SWITCH1_HAS_TRI 1
#define SWITCH1_IRQ -1
#define SWITCH1_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SWITCH1_IRQ_TYPE "NONE"
#define SWITCH1_NAME "/dev/switch1"
#define SWITCH1_RESET_VALUE 0
#define SWITCH1_SPAN 16
#define SWITCH1_TYPE "altera_avalon_pio"


/*
 * sysid_qsys_0 configuration
 *
 */

#define ALT_MODULE_CLASS_sysid_qsys_0 altera_avalon_sysid_qsys
#define SYSID_QSYS_0_BASE 0x222e0
#define SYSID_QSYS_0_ID 0
#define SYSID_QSYS_0_IRQ -1
#define SYSID_QSYS_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SYSID_QSYS_0_NAME "/dev/sysid_qsys_0"
#define SYSID_QSYS_0_SPAN 8
#define SYSID_QSYS_0_TIMESTAMP 1495498896
#define SYSID_QSYS_0_TYPE "altera_avalon_sysid_qsys"


/*
 * timer_0 configuration
 *
 */

#define ALT_MODULE_CLASS_timer_0 altera_avalon_timer
#define TIMER_0_ALWAYS_RUN 0
#define TIMER_0_BASE 0x22260
#define TIMER_0_COUNTER_SIZE 32
#define TIMER_0_FIXED_PERIOD 0
#define TIMER_0_FREQ 50000000
#define TIMER_0_IRQ 0
#define TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TIMER_0_LOAD_VALUE 49999
#define TIMER_0_MULT 0.001
#define TIMER_0_NAME "/dev/timer_0"
#define TIMER_0_PERIOD 1
#define TIMER_0_PERIOD_UNITS "ms"
#define TIMER_0_RESET_OUTPUT 0
#define TIMER_0_SNAPSHOT 1
#define TIMER_0_SPAN 32
#define TIMER_0_TICKS_PER_SEC 1000
#define TIMER_0_TIMEOUT_PULSE_OUTPUT 0
#define TIMER_0_TYPE "altera_avalon_timer"


/*
 * timer_1 configuration
 *
 */

#define ALT_MODULE_CLASS_timer_1 altera_avalon_timer
#define TIMER_1_ALWAYS_RUN 0
#define TIMER_1_BASE 0x22240
#define TIMER_1_COUNTER_SIZE 32
#define TIMER_1_FIXED_PERIOD 0
#define TIMER_1_FREQ 50000000
#define TIMER_1_IRQ 1
#define TIMER_1_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TIMER_1_LOAD_VALUE 49999
#define TIMER_1_MULT 0.001
#define TIMER_1_NAME "/dev/timer_1"
#define TIMER_1_PERIOD 1
#define TIMER_1_PERIOD_UNITS "ms"
#define TIMER_1_RESET_OUTPUT 0
#define TIMER_1_SNAPSHOT 1
#define TIMER_1_SPAN 32
#define TIMER_1_TICKS_PER_SEC 1000
#define TIMER_1_TIMEOUT_PULSE_OUTPUT 0
#define TIMER_1_TYPE "altera_avalon_timer"

#endif /* __SYSTEM_H_ */
