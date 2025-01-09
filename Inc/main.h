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
/*!
 * @addtogroup constants
 * @{
 */

/*! @} */
/*==================================================================================================
 *                                       DEFINES AND MACROS
 ==================================================================================================*/
/*!
 * @addtogroup defines_local_macros
 * @{
 */

#define INTERRUPT_DISABLE() do{ __asm volatile ("MOV R0, #1"); __asm volatile ("MSR PRIMASK, R0"); } while(0);

#define INTERRUPT_ENABLE()  do{ __asm volatile ("MOV R0, #0"); __asm volatile ("MSR PRIMASK, R0"); } while(0);

/* some stack memory calculations */
#define SIZE_TASK_STACK 		1024U
#define SIZE_SCHEDULER_STACK	1024U

#define SRAM_START				0x20000000U
#define SIZE_SRAM				( (128) * (1024))
#define SRAM_END				((SRAM_START) + (SIZE_SRAM))

#define T1_STACK_START			SRAM_END
#define T2_STACK_START			((SRAM_END) - (1 * SIZE_TASK_STACK))
#define T3_STACK_START			((SRAM_END) - (2 * SIZE_TASK_STACK))
#define T4_STACK_START			((SRAM_END) - (3 * SIZE_TASK_STACK))
#define IDLE_STACK_START		((SRAM_END) - (4 * SIZE_TASK_STACK))
#define SCHEDULER_STACK_START	((SRAM_END) - (5 * SIZE_TASK_STACK))

#define TICK_HZ 				1000U

#define HSI_CLOCK				16000000U
#define SYSTICK_TIM_CLK 		HSI_CLOCK

#define DUMMY_XPSR				0x01000000
#define LR_PSP_TO_PSP			0xFFFFFFFD

#define RELOAD_VALUE_OFFSET		1 /* The * RELOAD value is calculated according to its use. For example, to generate a multi-shot
timer with a period of N processor clock cycles, use a RELOAD value of N-1. */

#define INITIAL_ZERO			0

/*! @} */
/*==================================================================================================
 *                                             ENUMS
 ==================================================================================================*/
/*!
 * @addtogroup enums
 * @{
 */


/*!
 * @brief   Enumeration for task indexes in the system.
 * @details
 *          This enumeration defines the unique indexes for tasks in the system, including
 *          the idle task and other user-defined tasks. These indexes are used for identifying
 *          and managing tasks within the task scheduler.
 */
typedef enum
{
	IDLETASK        = 0, /*!< Index for the idle task, which runs when no other tasks are ready. */
	TASK1           = 1, /*!< Index for Task 1. */
	TASK2           = 2, /*!< Index for Task 2. */
	TASK3           = 3, /*!< Index for Task 3. */
	TASK4           = 4, /*!< Index for Task 4. */
	NUMBEROFTASK    = 5  /*!< Total number of tasks, including the idle task. */
}eTaskIndex_t;

/*!
 * @brief   Enumeration for CPU registers and stack frame indexes.
 * @details
 *          This enumeration defines the indexes of CPU registers and the number of
 *          instructions (or stack frame slots) used during task context switching in
 *          a multitasking system. These indexes are typically used for stack management
 *          when saving and restoring the context of a task.
 */
typedef enum
{
	R12 			= 0,	/*!< Index for R12 register in the stack frame. */
	R3              = 1,    /*!< Index for R3 register in the stack frame. */
	R2              = 2,    /*!< Index for R2 register in the stack frame. */
	R1              = 3,    /*!< Index for R1 register in the stack frame. */
	R0              = 4,    /*!< Index for R0 register in the stack frame. */
	R11             = 5,    /*!< Index for R11 register in the stack frame. */
	R10             = 6,    /*!< Index for R10 register in the stack frame. */
	R9              = 7,    /*!< Index for R9 register in the stack frame. */
	R8              = 8,    /*!< Index for R8 register in the stack frame. */
	R7              = 9,    /*!< Index for R7 register in the stack frame. */
	R6              = 10,   /*!< Index for R6 register in the stack frame. */
	R5              = 11,   /*!< Index for R5 register in the stack frame. */
	R4              = 12,   /*!< Index for R4 register in the stack frame. */
	NUMBEROFINST 	= 13    /*!< Total number of stack slots or registers saved/restored. */
}eInstructionIndex_t;

/*!
 * @brief   Task state enumeration.
 * @details
 *          This enumeration represents the possible states of a task in a multitasking
 *          system. It is used by the task scheduler to manage task execution based on
 *          their current state.
 */
typedef enum
{
  TASK_READY_STATE   = 0, /*!< The task is ready to run and is waiting in the scheduler's queue. */
  TASK_BLOCKED_STATE = 1  /*!< The task is blocked and waiting for a condition (e.g., delay or I/O). */
}current_state_t;

/*! @} */
/*==================================================================================================
 *                                 STRUCTURES AND OTHER TYPEDEFS
 ==================================================================================================*/
/*!
 * @addtogroup structures_typedefs
 * @{
 */

/*!
 * @brief   Task Control Block (TCB) structure.
 * @details
 *          The Task Control Block (TCB) is a data structure used to manage and store
 *          information about individual tasks in a multitasking system. It keeps
 *          track of the state and relevant information of a task, such as its stack pointer,
 *          block count, execution state, and function handler. This structure is essential
 *          for task scheduling and management in RTOS or bare-metal systems.
 */
typedef struct
{
  uint32 psp_value;	/*!< Process Stack Pointer value for the task. */
  uint32 block_count; /*!< Block count indicating the remaining time for which the task is blocked. */
  current_state_t current_state; /*!< Current state of the task (e.g., READY, BLOCKED, RUNNING). */
  void (*task_handler)(void); /*!< Pointer to the function that implements the task. */
}TCB_t;

/*! @} */
/*==================================================================================================
 *                                 GLOBAL VARIABLE DECLARATIONS
 ==================================================================================================*/
/*!
 * @addtogroup global_variables
 * @{
 */

/*! @} */
/*==================================================================================================
 *                                     FUNCTION PROTOTYPES
 ==================================================================================================*/
/*!
 * @addtogroup function_prototypes
 * @{
 */

/*! @} */
#endif /* MAIN_H_ */
