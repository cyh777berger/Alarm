/*
 * system.h - SOPC Builder system and BSP software package information
 *
 * Machine generated for CPU 'cpu1' in SOPC Builder design 'Alarm'
 * SOPC Builder design path: D:/alarm/qsys/hardware/Alarm.sopcinfo
 *
 * Generated: Thu Apr 24 19:50:49 CST 2025
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
#define ALT_CPU_BREAK_ADDR 0x000c0820
#define ALT_CPU_CPU_ARCH_NIOS2_R1
#define ALT_CPU_CPU_FREQ 100000000u
#define ALT_CPU_CPU_ID_SIZE 1
#define ALT_CPU_CPU_ID_VALUE 0x00000000
#define ALT_CPU_CPU_IMPLEMENTATION "tiny"
#define ALT_CPU_DATA_ADDR_WIDTH 0x14
#define ALT_CPU_DCACHE_LINE_SIZE 0
#define ALT_CPU_DCACHE_LINE_SIZE_LOG2 0
#define ALT_CPU_DCACHE_SIZE 0
#define ALT_CPU_EXCEPTION_ADDR 0x00040020
#define ALT_CPU_FLASH_ACCELERATOR_LINES 0
#define ALT_CPU_FLASH_ACCELERATOR_LINE_SIZE 0
#define ALT_CPU_FLUSHDA_SUPPORTED
#define ALT_CPU_FREQ 100000000
#define ALT_CPU_HARDWARE_DIVIDE_PRESENT 0
#define ALT_CPU_HARDWARE_MULTIPLY_PRESENT 0
#define ALT_CPU_HARDWARE_MULX_PRESENT 0
#define ALT_CPU_HAS_DEBUG_CORE 1
#define ALT_CPU_HAS_DEBUG_STUB
#define ALT_CPU_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define ALT_CPU_HAS_JMPI_INSTRUCTION
#define ALT_CPU_ICACHE_LINE_SIZE 0
#define ALT_CPU_ICACHE_LINE_SIZE_LOG2 0
#define ALT_CPU_ICACHE_SIZE 0
#define ALT_CPU_INST_ADDR_WIDTH 0x14
#define ALT_CPU_NAME "cpu1"
#define ALT_CPU_OCI_VERSION 1
#define ALT_CPU_RESET_ADDR 0x000a0000


/*
 * CPU configuration (with legacy prefix - don't use these anymore)
 *
 */

#define NIOS2_BIG_ENDIAN 0
#define NIOS2_BREAK_ADDR 0x000c0820
#define NIOS2_CPU_ARCH_NIOS2_R1
#define NIOS2_CPU_FREQ 100000000u
#define NIOS2_CPU_ID_SIZE 1
#define NIOS2_CPU_ID_VALUE 0x00000000
#define NIOS2_CPU_IMPLEMENTATION "tiny"
#define NIOS2_DATA_ADDR_WIDTH 0x14
#define NIOS2_DCACHE_LINE_SIZE 0
#define NIOS2_DCACHE_LINE_SIZE_LOG2 0
#define NIOS2_DCACHE_SIZE 0
#define NIOS2_EXCEPTION_ADDR 0x00040020
#define NIOS2_FLASH_ACCELERATOR_LINES 0
#define NIOS2_FLASH_ACCELERATOR_LINE_SIZE 0
#define NIOS2_FLUSHDA_SUPPORTED
#define NIOS2_HARDWARE_DIVIDE_PRESENT 0
#define NIOS2_HARDWARE_MULTIPLY_PRESENT 0
#define NIOS2_HARDWARE_MULX_PRESENT 0
#define NIOS2_HAS_DEBUG_CORE 1
#define NIOS2_HAS_DEBUG_STUB
#define NIOS2_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define NIOS2_HAS_JMPI_INSTRUCTION
#define NIOS2_ICACHE_LINE_SIZE 0
#define NIOS2_ICACHE_LINE_SIZE_LOG2 0
#define NIOS2_ICACHE_SIZE 0
#define NIOS2_INST_ADDR_WIDTH 0x14
#define NIOS2_OCI_VERSION 1
#define NIOS2_RESET_ADDR 0x000a0000


/*
 * Define for each module class mastered by the CPU
 *
 */

#define __ALTERA_AVALON_JTAG_UART
#define __ALTERA_AVALON_LCD_16207
#define __ALTERA_AVALON_ONCHIP_MEMORY2
#define __ALTERA_AVALON_PIO
#define __ALTERA_AVALON_SYSID_QSYS
#define __ALTERA_AVALON_TIMER
#define __ALTERA_NIOS2_GEN2


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
#define ALT_STDERR_BASE 0xc10b8
#define ALT_STDERR_DEV jtag_uart_0
#define ALT_STDERR_IS_JTAG_UART
#define ALT_STDERR_PRESENT
#define ALT_STDERR_TYPE "altera_avalon_jtag_uart"
#define ALT_STDIN "/dev/jtag_uart_0"
#define ALT_STDIN_BASE 0xc10b8
#define ALT_STDIN_DEV jtag_uart_0
#define ALT_STDIN_IS_JTAG_UART
#define ALT_STDIN_PRESENT
#define ALT_STDIN_TYPE "altera_avalon_jtag_uart"
#define ALT_STDOUT "/dev/jtag_uart_0"
#define ALT_STDOUT_BASE 0xc10b8
#define ALT_STDOUT_DEV jtag_uart_0
#define ALT_STDOUT_IS_JTAG_UART
#define ALT_STDOUT_PRESENT
#define ALT_STDOUT_TYPE "altera_avalon_jtag_uart"
#define ALT_SYSTEM_NAME "Alarm"


/*
 * button1 configuration
 *
 */

#define ALT_MODULE_CLASS_button1 altera_avalon_pio
#define BUTTON1_BASE 0xc1070
#define BUTTON1_BIT_CLEARING_EDGE_REGISTER 1
#define BUTTON1_BIT_MODIFYING_OUTPUT_REGISTER 0
#define BUTTON1_CAPTURE 1
#define BUTTON1_DATA_WIDTH 1
#define BUTTON1_DO_TEST_BENCH_WIRING 0
#define BUTTON1_DRIVEN_SIM_VALUE 0
#define BUTTON1_EDGE_TYPE "RISING"
#define BUTTON1_FREQ 100000000
#define BUTTON1_HAS_IN 1
#define BUTTON1_HAS_OUT 0
#define BUTTON1_HAS_TRI 0
#define BUTTON1_IRQ 1
#define BUTTON1_IRQ_INTERRUPT_CONTROLLER_ID 0
#define BUTTON1_IRQ_TYPE "EDGE"
#define BUTTON1_NAME "/dev/button1"
#define BUTTON1_RESET_VALUE 0
#define BUTTON1_SPAN 16
#define BUTTON1_TYPE "altera_avalon_pio"


/*
 * button2 configuration
 *
 */

#define ALT_MODULE_CLASS_button2 altera_avalon_pio
#define BUTTON2_BASE 0xc1060
#define BUTTON2_BIT_CLEARING_EDGE_REGISTER 1
#define BUTTON2_BIT_MODIFYING_OUTPUT_REGISTER 0
#define BUTTON2_CAPTURE 1
#define BUTTON2_DATA_WIDTH 1
#define BUTTON2_DO_TEST_BENCH_WIRING 0
#define BUTTON2_DRIVEN_SIM_VALUE 0
#define BUTTON2_EDGE_TYPE "RISING"
#define BUTTON2_FREQ 100000000
#define BUTTON2_HAS_IN 1
#define BUTTON2_HAS_OUT 0
#define BUTTON2_HAS_TRI 0
#define BUTTON2_IRQ 2
#define BUTTON2_IRQ_INTERRUPT_CONTROLLER_ID 0
#define BUTTON2_IRQ_TYPE "EDGE"
#define BUTTON2_NAME "/dev/button2"
#define BUTTON2_RESET_VALUE 0
#define BUTTON2_SPAN 16
#define BUTTON2_TYPE "altera_avalon_pio"


/*
 * button3 configuration
 *
 */

#define ALT_MODULE_CLASS_button3 altera_avalon_pio
#define BUTTON3_BASE 0xc1020
#define BUTTON3_BIT_CLEARING_EDGE_REGISTER 1
#define BUTTON3_BIT_MODIFYING_OUTPUT_REGISTER 0
#define BUTTON3_CAPTURE 1
#define BUTTON3_DATA_WIDTH 1
#define BUTTON3_DO_TEST_BENCH_WIRING 0
#define BUTTON3_DRIVEN_SIM_VALUE 0
#define BUTTON3_EDGE_TYPE "RISING"
#define BUTTON3_FREQ 100000000
#define BUTTON3_HAS_IN 1
#define BUTTON3_HAS_OUT 0
#define BUTTON3_HAS_TRI 0
#define BUTTON3_IRQ 3
#define BUTTON3_IRQ_INTERRUPT_CONTROLLER_ID 0
#define BUTTON3_IRQ_TYPE "EDGE"
#define BUTTON3_NAME "/dev/button3"
#define BUTTON3_RESET_VALUE 0
#define BUTTON3_SPAN 16
#define BUTTON3_TYPE "altera_avalon_pio"


/*
 * display configuration
 *
 */

#define ALT_MODULE_CLASS_display altera_avalon_pio
#define DISPLAY_BASE 0xc1040
#define DISPLAY_BIT_CLEARING_EDGE_REGISTER 0
#define DISPLAY_BIT_MODIFYING_OUTPUT_REGISTER 0
#define DISPLAY_CAPTURE 0
#define DISPLAY_DATA_WIDTH 32
#define DISPLAY_DO_TEST_BENCH_WIRING 0
#define DISPLAY_DRIVEN_SIM_VALUE 0
#define DISPLAY_EDGE_TYPE "NONE"
#define DISPLAY_FREQ 100000000
#define DISPLAY_HAS_IN 0
#define DISPLAY_HAS_OUT 1
#define DISPLAY_HAS_TRI 0
#define DISPLAY_IRQ -1
#define DISPLAY_IRQ_INTERRUPT_CONTROLLER_ID -1
#define DISPLAY_IRQ_TYPE "NONE"
#define DISPLAY_NAME "/dev/display"
#define DISPLAY_RESET_VALUE 0
#define DISPLAY_SPAN 16
#define DISPLAY_TYPE "altera_avalon_pio"


/*
 * display_2 configuration
 *
 */

#define ALT_MODULE_CLASS_display_2 altera_avalon_pio
#define DISPLAY_2_BASE 0xc1030
#define DISPLAY_2_BIT_CLEARING_EDGE_REGISTER 0
#define DISPLAY_2_BIT_MODIFYING_OUTPUT_REGISTER 0
#define DISPLAY_2_CAPTURE 0
#define DISPLAY_2_DATA_WIDTH 16
#define DISPLAY_2_DO_TEST_BENCH_WIRING 0
#define DISPLAY_2_DRIVEN_SIM_VALUE 0
#define DISPLAY_2_EDGE_TYPE "NONE"
#define DISPLAY_2_FREQ 100000000
#define DISPLAY_2_HAS_IN 0
#define DISPLAY_2_HAS_OUT 1
#define DISPLAY_2_HAS_TRI 0
#define DISPLAY_2_IRQ -1
#define DISPLAY_2_IRQ_INTERRUPT_CONTROLLER_ID -1
#define DISPLAY_2_IRQ_TYPE "NONE"
#define DISPLAY_2_NAME "/dev/display_2"
#define DISPLAY_2_RESET_VALUE 0
#define DISPLAY_2_SPAN 16
#define DISPLAY_2_TYPE "altera_avalon_pio"


/*
 * hal configuration
 *
 */

#define ALT_MAX_FD 32
#define ALT_SYS_CLK TIMER_0
#define ALT_TIMESTAMP_CLK none


/*
 * jtag_uart_0 configuration
 *
 */

#define ALT_MODULE_CLASS_jtag_uart_0 altera_avalon_jtag_uart
#define JTAG_UART_0_BASE 0xc10b8
#define JTAG_UART_0_IRQ 0
#define JTAG_UART_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define JTAG_UART_0_NAME "/dev/jtag_uart_0"
#define JTAG_UART_0_READ_DEPTH 64
#define JTAG_UART_0_READ_THRESHOLD 8
#define JTAG_UART_0_SPAN 8
#define JTAG_UART_0_TYPE "altera_avalon_jtag_uart"
#define JTAG_UART_0_WRITE_DEPTH 64
#define JTAG_UART_0_WRITE_THRESHOLD 8


/*
 * lcd_16207_0 configuration
 *
 */

#define ALT_MODULE_CLASS_lcd_16207_0 altera_avalon_lcd_16207
#define LCD_16207_0_BASE 0xc10a0
#define LCD_16207_0_IRQ -1
#define LCD_16207_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define LCD_16207_0_NAME "/dev/lcd_16207_0"
#define LCD_16207_0_SPAN 16
#define LCD_16207_0_TYPE "altera_avalon_lcd_16207"


/*
 * ledg configuration
 *
 */

#define ALT_MODULE_CLASS_ledg altera_avalon_pio
#define LEDG_BASE 0xc1080
#define LEDG_BIT_CLEARING_EDGE_REGISTER 0
#define LEDG_BIT_MODIFYING_OUTPUT_REGISTER 0
#define LEDG_CAPTURE 0
#define LEDG_DATA_WIDTH 1
#define LEDG_DO_TEST_BENCH_WIRING 0
#define LEDG_DRIVEN_SIM_VALUE 0
#define LEDG_EDGE_TYPE "NONE"
#define LEDG_FREQ 100000000
#define LEDG_HAS_IN 0
#define LEDG_HAS_OUT 1
#define LEDG_HAS_TRI 0
#define LEDG_IRQ -1
#define LEDG_IRQ_INTERRUPT_CONTROLLER_ID -1
#define LEDG_IRQ_TYPE "NONE"
#define LEDG_NAME "/dev/ledg"
#define LEDG_RESET_VALUE 0
#define LEDG_SPAN 16
#define LEDG_TYPE "altera_avalon_pio"


/*
 * ledr configuration
 *
 */

#define ALT_MODULE_CLASS_ledr altera_avalon_pio
#define LEDR_BASE 0xc1090
#define LEDR_BIT_CLEARING_EDGE_REGISTER 0
#define LEDR_BIT_MODIFYING_OUTPUT_REGISTER 0
#define LEDR_CAPTURE 0
#define LEDR_DATA_WIDTH 2
#define LEDR_DO_TEST_BENCH_WIRING 0
#define LEDR_DRIVEN_SIM_VALUE 0
#define LEDR_EDGE_TYPE "NONE"
#define LEDR_FREQ 100000000
#define LEDR_HAS_IN 0
#define LEDR_HAS_OUT 1
#define LEDR_HAS_TRI 0
#define LEDR_IRQ -1
#define LEDR_IRQ_INTERRUPT_CONTROLLER_ID -1
#define LEDR_IRQ_TYPE "NONE"
#define LEDR_NAME "/dev/ledr"
#define LEDR_RESET_VALUE 0
#define LEDR_SPAN 16
#define LEDR_TYPE "altera_avalon_pio"


/*
 * ram configuration
 *
 */

#define ALT_MODULE_CLASS_ram altera_avalon_onchip_memory2
#define RAM_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define RAM_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define RAM_BASE 0x40000
#define RAM_CONTENTS_INFO ""
#define RAM_DUAL_PORT 0
#define RAM_GUI_RAM_BLOCK_TYPE "AUTO"
#define RAM_INIT_CONTENTS_FILE "Alarm_ram"
#define RAM_INIT_MEM_CONTENT 1
#define RAM_INSTANCE_ID "NONE"
#define RAM_IRQ -1
#define RAM_IRQ_INTERRUPT_CONTROLLER_ID -1
#define RAM_NAME "/dev/ram"
#define RAM_NON_DEFAULT_INIT_FILE_ENABLED 0
#define RAM_RAM_BLOCK_TYPE "AUTO"
#define RAM_READ_DURING_WRITE_MODE "DONT_CARE"
#define RAM_SINGLE_CLOCK_OP 0
#define RAM_SIZE_MULTIPLE 1
#define RAM_SIZE_VALUE 204800
#define RAM_SPAN 204800
#define RAM_TYPE "altera_avalon_onchip_memory2"
#define RAM_WRITABLE 1


/*
 * rom configuration
 *
 */

#define ALT_MODULE_CLASS_rom altera_avalon_onchip_memory2
#define ROM_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define ROM_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define ROM_BASE 0xa0000
#define ROM_CONTENTS_INFO ""
#define ROM_DUAL_PORT 0
#define ROM_GUI_RAM_BLOCK_TYPE "AUTO"
#define ROM_INIT_CONTENTS_FILE "Alarm_rom"
#define ROM_INIT_MEM_CONTENT 1
#define ROM_INSTANCE_ID "NONE"
#define ROM_IRQ -1
#define ROM_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ROM_NAME "/dev/rom"
#define ROM_NON_DEFAULT_INIT_FILE_ENABLED 0
#define ROM_RAM_BLOCK_TYPE "AUTO"
#define ROM_READ_DURING_WRITE_MODE "DONT_CARE"
#define ROM_SINGLE_CLOCK_OP 0
#define ROM_SIZE_MULTIPLE 1
#define ROM_SIZE_VALUE 102400
#define ROM_SPAN 102400
#define ROM_TYPE "altera_avalon_onchip_memory2"
#define ROM_WRITABLE 0


/*
 * sliders configuration
 *
 */

#define ALT_MODULE_CLASS_sliders altera_avalon_pio
#define SLIDERS_BASE 0xc1050
#define SLIDERS_BIT_CLEARING_EDGE_REGISTER 0
#define SLIDERS_BIT_MODIFYING_OUTPUT_REGISTER 0
#define SLIDERS_CAPTURE 0
#define SLIDERS_DATA_WIDTH 4
#define SLIDERS_DO_TEST_BENCH_WIRING 0
#define SLIDERS_DRIVEN_SIM_VALUE 0
#define SLIDERS_EDGE_TYPE "NONE"
#define SLIDERS_FREQ 100000000
#define SLIDERS_HAS_IN 1
#define SLIDERS_HAS_OUT 0
#define SLIDERS_HAS_TRI 0
#define SLIDERS_IRQ -1
#define SLIDERS_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SLIDERS_IRQ_TYPE "NONE"
#define SLIDERS_NAME "/dev/sliders"
#define SLIDERS_RESET_VALUE 0
#define SLIDERS_SPAN 16
#define SLIDERS_TYPE "altera_avalon_pio"


/*
 * sysid_qsys_0 configuration
 *
 */

#define ALT_MODULE_CLASS_sysid_qsys_0 altera_avalon_sysid_qsys
#define SYSID_QSYS_0_BASE 0xc10b0
#define SYSID_QSYS_0_ID 0
#define SYSID_QSYS_0_IRQ -1
#define SYSID_QSYS_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SYSID_QSYS_0_NAME "/dev/sysid_qsys_0"
#define SYSID_QSYS_0_SPAN 8
#define SYSID_QSYS_0_TIMESTAMP 1745495106
#define SYSID_QSYS_0_TYPE "altera_avalon_sysid_qsys"


/*
 * timer_0 configuration
 *
 */

#define ALT_MODULE_CLASS_timer_0 altera_avalon_timer
#define TIMER_0_ALWAYS_RUN 0
#define TIMER_0_BASE 0xc1000
#define TIMER_0_COUNTER_SIZE 32
#define TIMER_0_FIXED_PERIOD 0
#define TIMER_0_FREQ 100000000
#define TIMER_0_IRQ 4
#define TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TIMER_0_LOAD_VALUE 99999
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

#endif /* __SYSTEM_H_ */
