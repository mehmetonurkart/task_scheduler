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

#define TICK_HZ 			1000U

#define HSI_CLOCK			16000000U
#define SYSTICK_TIM_CLK 	HSI_CLOCK

#define RELOAD_VALUE_OFFSET	1 /* The * RELOAD value is calculated according to its use. For example, to generate a multi-shot
timer with a period of N processor clock cycles, use a RELOAD value of N-1. */

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
	init_systick_timer(TICK_HZ);

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
void SysTick_Handler (void)
{

}

/*! @} */
