/*!**************************************************************************************************
 * @name : task_scheduler
 * @file : main.c
 * @date : 03.01.2025
 * @author : mehmtonrr
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
/* Scheduler and Task Design in RAM
RAM_START																																			                                                             RAM_END
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|------------------------------------------------------------|------------------------------|-----------------------------|-----------------------------|-----------------------------|-----------------------------|
|                                                            | Private stack scheduler      | Private stack T4            | Private stack T3            | Private stack T2            | Private stack T1            |
|                                                            | (1 KB)                       |      (1 KB)                 |      (1 KB)                 | (1 KB)                      | (1 KB)                      |
|------------------------------------------------------------|------------------------------|-----------------------------|-----------------------------|-----------------------------|-----------------------------|
<--------------------------------------------------------------------------------------------128KB of RAM(SRAM1+SRAM2)---------------------------------------------------------------------------------------------->
+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

 Scheduling Policy Selection
* We will be using round robin pre-emptive scheduling.
* No task priority.
* We will use systick timer to generate exception for every 1ms to run the scheduler code.

      	       ^T1		^T2	    ^T3	     ^T4	  ^T1	    ^T2
      |        |        |        |        |        |        |
      |--------|--------|--------|--------|--------|--------|
      |        |        |        |        |        |        |
      |        |        |        |        |        |        |
      | 1 ms   | 1 ms   | 1 ms   | 1 ms   | 1 ms   | 1 ms   |
---------------------------------------------------------------
<---------------- Systick exception every 1 ms ---------------->

*/
/*==================================================================================================
 *                                        INCLUDE FILES
 * 1) system and project includes
 * 2) needed interfaces from external units
 * 3) internal and external interfaces from this unit
 ==================================================================================================*/
#include <stdint.h>
#include <stdio.h>
#include "StandartTypes.h"
#include "main.h"


#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif
/*==================================================================================================
 *                                       LOCAL MACROS
 ==================================================================================================*/
/*!
 * @addtogroup define_local_macros
 * @{
 */

/*! @} */
/*==================================================================================================
 *                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 ==================================================================================================*/
/*!
 * @addtogroup define_local_typedefs
 * @{
 */

/*! @} */
/*==================================================================================================
 *                                       LOCAL VARIABLES
 ==================================================================================================*/
/*!
 * @addtogroup define_local_variables
 * @{
 */

/*! @} */
/*==================================================================================================
 *                                      GLOBAL VARIABLES
 ==================================================================================================*/
/*!
 * @addtogroup define_global_variables
 * @{
 */
uint32 psp_of_tasks[MAX_TASKS] = {T1_STACK_START, T2_STACK_START, T3_STACK_START, T4_STACK_START};
uint32 task_handlers[MAX_TASKS];
uint8 current_task = TASK1; /*! Task1 is running */

/*! @} */
/*==================================================================================================
 *                                    FUNCTION PROTOTYPES
 ==================================================================================================*/
/*!
 * @addtogroup define_function_prototypes
 * @{
 */

void task1_handler(void); /*!  This is task 1 */
void task2_handler(void); /*!  This is task 2 */
void task3_handler(void); /*!  This is task 3 */
void task4_handler(void); /*!  This is task 4 of the application */

void init_systick_timer(uint32 tick_hz);
__attribute__ ((naked)) void init_scheduler_stack(uint32 sched_top_of_stack);
void init_tasks_stack (void);
void enable_processor_faults(void);
uint32 get_psp_value (void);
__attribute__((naked)) void switch_sp_to_psp (void);

/*! @} */
/*==================================================================================================
 *                                       LOCAL FUNCTIONS
 ==================================================================================================*/
/*!
 * @addtogroup define_local_functions
 * @{
 */

/*! @} */
/*==================================================================================================
 *                                         FUNCTIONS
 ==================================================================================================*/
/*!
 * @addtogroup define_functions
 * @{
 */

/********************************************************************************
*@brief   None
*@details None
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note       None
*@return     None
********************************************************************************/
int main(void)
{
	enable_processor_faults();

	init_scheduler_stack(SCHEDULER_STACK_START);

	task_handlers[TASK1] = (uint32 ) task1_handler;
	task_handlers[TASK2] = (uint32 ) task2_handler;
	task_handlers[TASK3] = (uint32 ) task3_handler;
	task_handlers[TASK4] = (uint32 ) task4_handler;

	init_tasks_stack();

	init_systick_timer(TICK_HZ);

	switch_sp_to_psp();

	task1_handler();

	for(;;);
}

/********************************************************************************
*@brief   None
*@details None
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note       None
*@return     None
********************************************************************************/
void task1_handler(void)
{
	while(1)
	{
		printf("This is task1\n");
	}
}

/********************************************************************************
*@brief   None
*@details None
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note       None
*@return     None
********************************************************************************/
void task2_handler(void)
{
	while(1)
	{
		printf("This is task2\n");
	}
}

/********************************************************************************
*@brief   None
*@details None
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note       None
*@return     None
********************************************************************************/
void task3_handler(void)
{
	while(1)
	{
		printf("This is task3\n");
	}
}

/********************************************************************************
*@brief   None
*@details None
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note       None
*@return     None
********************************************************************************/
void task4_handler(void)
{
	while(1)
	{
		printf("This is task4\n");
	}
}

/********************************************************************************
*@brief   None
*@details None
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note       None
*@return     None
********************************************************************************/
void init_systick_timer(uint32 tick_hz)
{
	uint32 *pSRVR = (uint32 *) 0xE000E014; /*! SysTick Reload Value Register Address  */
	uint32 *pSCSR = (uint32 *) 0xE000E010; /*! SysTick Current Value Register Address  */
	uint32 count_value = (SYSTICK_TIM_CLK / tick_hz) - RELOAD_VALUE_OFFSET;

	/*!  Clear the value of SVR */
	*pSRVR &= ~(0x00FFFFFFFF);

	/*!  load the value in to SVR */
	*pSRVR |= count_value;

	/*!  do some settings */
	*pSCSR |= (1 << 1); /*! Enable Systick exception request */
	*pSCSR |= (1 << 2); /*! Indicates the clock source, processor clock source */

	/*!  enable the systick */
	*pSCSR |= (1 << 0); /*! Enable the counter */
}


/********************************************************************************
*@brief   None
*@details None
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note       None
*@return     None
********************************************************************************/
__attribute__ ((naked)) void init_scheduler_stack(uint32 sched_top_of_stack)
{
	__asm volatile ("MSR MSP,%0": : "r" (sched_top_of_stack) : );
	__asm volatile ("BX LR"); /*! Function call.We will be function call because this function(init_scheduler_stack) is naked function. */
}

/********************************************************************************
*@brief   None
*@details None
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note       None
*@return     None
********************************************************************************/
void init_tasks_stack (void)
{

	uint32 *pPSP;

	for(uint8 TaskIndex = TASK1; TaskIndex < NUMBEROFTASK; TaskIndex++ )
	{
		pPSP = (uint32 *)psp_of_tasks[TaskIndex];

		pPSP--; /*! xPSR */
		*pPSP = DUMMY_XPSR; /*! 0x00100000 */

		pPSP--; /*! PC */
		*pPSP = task_handlers[TaskIndex];

		pPSP--; /*! LR */
		*pPSP = LR_PSP_TO_PSP;

		for(uint8 InstIndex = 0; InstIndex < NUMBEROFINST; InstIndex++ )
		{
			pPSP--; /*! R12-R3-R2-R1-R0-R11-R10-R9-R8-R7-R6-R5-R4 */
			*pPSP = INITIAL_ZERO;
		}
	}
}

/********************************************************************************
*@brief   None
*@details None
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note       None
*@return     None
********************************************************************************/
void enable_processor_faults (void)
{
	/*! 1. Enable all configurable exceptions line usage fault, mem manage fault and bus fault */
	uint32_t *pSHCSR = (uint32_t*) 0xE000ED24;

	*pSHCSR |= (1 << 16); /*! mem manage */
	*pSHCSR |= (1 << 17); /*! bus fault */
	*pSHCSR |= (1 << 18); /*! usage fault */
}

/********************************************************************************
*@brief   None
*@details None
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note       None
*@return     None
********************************************************************************/
uint32 get_psp_value (void)
{

	return psp_of_tasks[current_task];
}

/********************************************************************************
*@brief   None
*@details None
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note       None
*@return     None
********************************************************************************/
__attribute__((naked)) void switch_sp_to_psp (void)
{
	/*! 1.Initialize the PSP with TASK1 stack start address */

	/*! Get the value of psp of current_task */
	__asm volatile ("PUSH {LR}"); /*! preserve LR which connects back to main(). */
	__asm volatile ("BL get_psp_value");
	__asm volatile ("MSR PSP,R0"); /*! Initialize PSP */
	__asm volatile ("POP {LR}"); /*! POP back LR value */

	/*! 2. Change SP to PSP using CONTROL register */
	__asm volatile ("MOV R0, #0X02");
	__asm volatile ("MSR CONTROL, R0");
	__asm volatile ("BX LR");

}
/********************************************************************************
*@brief   None
*@details None
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note       None
*@return     None
********************************************************************************/
void SysTick_Handler (void)
{
	/*! Save the context of current task */
	/*! 1.Get current running task's PSP value */
	/*! 2.Using that PSP value store SF2(R4 to R11) */
	/*! 3.Save the current value of PSP */

	/*! Retrieve the context of next task */
	/*! 1.Decide next task to run */
	/*! 2.Get its past PSP value */
	/*! 3.Using that PSP value retrieve SF2(R4 to R11) */
	/*! 4.Update PSP and exit */
}

/********************************************************************************
*@brief   None
*@details None
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note       None
*@return     None
********************************************************************************/
void HardFault_Handler(void)
{
	printf("Exception : HardFault\n");
	while (1);
}

/********************************************************************************
*@brief   None
*@details None
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note       None
*@return     None
********************************************************************************/
void MemManage_Handler(void)
{
	printf("Exception : MemManage\n");
	while (1);

}

/********************************************************************************
*@brief   None
*@details None
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note       None
*@return     None
********************************************************************************/
void BusFault_Handler(void)
{
	printf("Exception : BusFault\n");
	while (1);

}

/*! @} */
