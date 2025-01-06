/*!**************************************************************************************************
 * @name : task_scheduler
 * @file : main.h
 * @date : Jan 6, 2025
 * @author : mehmet.kart
 * @brief : None
 *
 * @version  x.x
 *
 *---------------------------------------------------------------------------------------------------
 *
 * (c) Copyright xxxx-xxxx XXXXXXXX.
 * Copyright xxxx-xxxx XXXXXXXX.
 * All Rights Reserved.
 *
 ***************************************************************************************************/
#ifndef MAIN_H_
#define MAIN_H_
/*==================================================================================================
 *                                         INCLUDE FILES
 * 1) system and project includes
 * 2) needed interfaces from external units
 * 3) internal and external interfaces from this unit
 ==================================================================================================*/

/*==================================================================================================
 *                                           CONSTANTS
 ==================================================================================================*/
/*==================================================================================================
 *                                       DEFINES AND MACROS
 ==================================================================================================*/
/* some stack memory calculations */
#define SIZE_TASK_STACK 		1024U
#define SIZE_SCHEDULER_STACK	1024U

#define SRAM_START				0x20000000U
#define SIZE_SRAM				(128 * (1024))
#define SRAM_END				((SRAM_START) + (SIZE_SRAM))

#define T1_STACK_START			(SRAM_END)
#define T2_STACK_START			((SRAM_END) - (1 * SIZE_TASK_STACK))
#define T3_STACK_START			((SRAM_END) - (2 * SIZE_TASK_STACK))
#define T4_STACK_START			((SRAM_END) - (3 * SIZE_TASK_STACK))
#define SCHEDULER_STACK_START	((SRAM_END) - (4 * SIZE_TASK_STACK))

#define TICK_HZ 				1000U

#define HSI_CLOCK				16000000U
#define SYSTICK_TIM_CLK 		HSI_CLOCK

#define DUMMY_XPSR				0x00100000
#define LR_PSP_TO_PSP			0xFFFFFFFD

#define RELOAD_VALUE_OFFSET		1 /* The * RELOAD value is calculated according to its use. For example, to generate a multi-shot
timer with a period of N processor clock cycles, use a RELOAD value of N-1. */

#define MAX_TASKS 				4
#define INITIAL_ZERO			0
/*==================================================================================================
 *                                             ENUMS
 ==================================================================================================*/
typedef enum
{
	TASK1 			= 0,
	TASK2 			= 1,
	TASK3 			= 2,
	TASK4 			= 3,
	NUMBEROFTASK 	= 4
}eTaskIndex_t;

typedef enum
{
	R12 			= 0,
	R3              = 1,
	R2              = 2,
	R1              = 3,
	R0              = 4,
	R11             = 5,
	R10             = 6,
	R9              = 7,
	R8              = 8,
	R7              = 9,
	R6              = 10,
	R5              = 11,
	R4              = 12,
	NUMBEROFINST 	= 13
}eInstructionIndex_t;
/*==================================================================================================
 *                                 STRUCTURES AND OTHER TYPEDEFS
 ==================================================================================================*/
/*==================================================================================================
 *                                 GLOBAL VARIABLE DECLARATIONS
 ==================================================================================================*/
/*==================================================================================================
 *                                     FUNCTION PROTOTYPES
 ==================================================================================================*/


#endif /* MAIN_H_ */
