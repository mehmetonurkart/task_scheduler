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
#include "led.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif
/*==================================================================================================
 *                                       DEFINES AND LOCAL MACROS
 ==================================================================================================*/
/*!
 * @addtogroup defines_local_macros
 * @{
 */

/*! @} */
/*==================================================================================================
 *                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 ==================================================================================================*/
/*!
 * @addtogroup local_typedefs
 * @{
 */

/*! @} */
/*==================================================================================================
 *                                       LOCAL VARIABLES
 ==================================================================================================*/
/*!
 * @addtogroup local_variables
 * @{
 */

/*! @} */
/*==================================================================================================
 *                                      GLOBAL VARIABLES
 ==================================================================================================*/
/*!
 * @addtogroup global_variables
 * @{
 */
/*!
 * @brief
 */

TCB_t user_tasks[NUMBEROFTASK];
uint8 current_task = TASK1; /*! Task1 is running */
uint32 g_tick_count = INITIAL_ZERO;

/*! @} */
/*==================================================================================================
 *                                    FUNCTION PROTOTYPES
 ==================================================================================================*/
/*!
 * @addtogroup function_prototypes
 * @{
 */

void idle_tasks(void); 	  /*!  This is task idle */
void task1_handler(void); /*!  This is task 1 */
void task2_handler(void); /*!  This is task 2 */
void task3_handler(void); /*!  This is task 3 */
void task4_handler(void); /*!  This is task 4 of the application */

void init_systick_timer(uint32 tick_hz);
__attribute__ ((naked)) void init_scheduler_stack(uint32 sched_top_of_stack);
void init_tasks_stack (void);
void enable_processor_faults(void);
uint32 get_psp_value (void);
void save_psp_value (uint32 current_psp_value);
void update_next_task (void);
__attribute__((naked)) void switch_sp_to_psp (void);
void schedule(void);
void task_delay(uint32 tick_count);
void update_global_tick_count(void);
void unblock_tasks(void);

/*! @} */
/*==================================================================================================
 *                                       LOCAL FUNCTIONS
 ==================================================================================================*/
/*!
 * @addtogroup local_functions
 * @{
 */

/*! @} */
/*==================================================================================================
 *                                         FUNCTIONS
 ==================================================================================================*/
/*!
 * @addtogroup functions
 * @{
 */

/********************************************************************************
*@brief   Main function to initialize the task scheduler and start the system.
*@details
*         - Processor faults are enabled for debugging.
*         - Scheduler stack is initialized at a specific memory address.
*         - Task handlers are assigned to appropriate task functions.
*         - Task stacks are initialized for each task.
*         - The system tick timer is configured for periodic interrupts.
*         - The processor stack pointer is switched to the process stack pointer (PSP).
*         - Execution begins with the first task.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note
*         Ensure that each task handler points to a valid task function.
*         This implementation assumes TASK1 to TASK4 macros and TICK_HZ constant
*         are properly defined in the project configuration.
*@return     None
********************************************************************************/
int main(void)
{
	enable_processor_faults();

	init_scheduler_stack(SCHEDULER_STACK_START);

	init_tasks_stack();

	led_init_all();

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
void idle_task(void)
{
  while(1);
}

/********************************************************************************
*@brief   Task 1 handler function.
*@details
*         The function represents the execution of Task 1. It enters an infinite
*         loop where it repeatedly outputs a message to the console to indicate
*         that Task 1 is running.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note
*         - Ensure the task scheduling mechanism allows proper context switching
*           so this task does not block other tasks.
*         - The use of `printf` may not be thread-safe in embedded environments;
*           consider using a safer alternative if multitasking is used.
*@return     None
********************************************************************************/
void task1_handler(void)
{
	while(1)
	{
		led_on(LED_GREEN);
		task_delay(DELAY_1S);
		led_off(LED_GREEN);
		task_delay(DELAY_1S);
	}
}

/********************************************************************************
*@brief   Task 2 handler function.
*@details
*         The function defines the behavior of Task 2. It enters an infinite loop
*         and continuously outputs a message to indicate that Task 2 is running.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note
*         - Proper task scheduling or time-slicing should ensure this function
*           does not block other tasks in a multitasking environment.
*         - Replace `printf` with a lightweight or thread-safe alternative for
*           real-time or resource-constrained systems.
*@return     None
********************************************************************************/
void task2_handler(void)
{
	while(1)
	{
		led_on(LED_ORANGE);
		task_delay(DELAY_500MS);
		led_off(LED_ORANGE);
		task_delay(DELAY_500MS);
	}
}

/********************************************************************************
*@brief   Task 3 handler function.
*@details
*         This function implements the execution logic for Task 3. It operates
*         in an infinite loop and outputs a message to indicate that Task 3 is
*         active and running.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note
*         - Ensure this function is managed within a task scheduler to prevent
*           monopolizing the processor resources.
*         - `printf` may introduce delays or synchronization issues; consider
*           using a lightweight or thread-safe alternative if required.
*@return     None
********************************************************************************/
void task3_handler(void)
{
	while(1)
	{
		led_on(LED_BLUE);
		task_delay(DELAY_250MS);
		led_off(LED_BLUE);
		task_delay(DELAY_250MS);
	}
}

/********************************************************************************
*@brief   Task 4 handler function.
*@details
*         This function defines the execution logic for Task 4. It continuously
*         executes in an infinite loop and prints a message to indicate Task 4
*         is active and running.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note
*         - This function should be part of a task scheduler or time-slicing
*           mechanism to ensure fair execution of other tasks.
*         - Use of `printf` in a real-time system may lead to blocking or delays;
*           consider alternatives suited for embedded environments.
*@return     None
********************************************************************************/
void task4_handler(void)
{
	while(1)
	{
		led_on(LED_RED);
		task_delay(DELAY_125MS);
		led_off(LED_RED);
		task_delay(DELAY_125MS);
	}
}

/********************************************************************************
*@brief   Initialize the SysTick timer with a specific tick frequency.
*@details
*         This function configures the SysTick timer to generate interrupts at a
*         frequency specified by the `tick_hz` parameter. It sets the reload value
*         for the timer, enables the SysTick exception, selects the processor clock
*         as the timer source, and enables the timer.
*********************************************************************************
*@param[in]  tick_hz: The desired tick frequency (in Hertz) of the SysTick timer.
*                     This determines the period between interrupts.
*@param[out] None
*@note
*         - Ensure that `SYSTICK_TIM_CLK` and `RELOAD_VALUE_OFFSET` are defined
*           appropriately in the system configuration.
*         - This configuration assumes the processor is using the system clock
*           as the source for the SysTick timer.
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
*@brief   Initialize the stack pointer for the scheduler.
*@details
*         This function initializes the Main Stack Pointer (MSP) to the value of
*         `sched_top_of_stack`, which is the top of the scheduler's stack. It uses
*         ARM assembly to perform this operation and then returns from the function.
*         The function is declared with the `naked` attribute, indicating no prologue
*         or epilogue code is inserted by the compiler.
*********************************************************************************
*@param[in]  sched_top_of_stack: The memory address of the top of the scheduler stack.
*                                 This address is used to set the MSP for task scheduling.
*@param[out] None
*@note
*         - The function is marked with the `naked` attribute, which prevents the
*           compiler from generating a prologue and epilogue. As a result, it doesn't
*           save and restore registers, so it must be used with caution.
*         - The function uses inline assembly, and the `MSR` instruction sets the MSP.
*           The `BX LR` instruction is used to return from the function.
*@return     None
********************************************************************************/
__attribute__ ((naked)) void init_scheduler_stack(uint32 sched_top_of_stack)
{
	__asm volatile ("MSR MSP,%0": : "r" (sched_top_of_stack) : );
	__asm volatile ("BX LR"); /*! Function call.We will be function call because this function(init_scheduler_stack) is naked function. */
}

/********************************************************************************
*@brief   Initialize the task stacks for all tasks.
*@details
*         This function sets up the stack for each task in the system. It assumes that
*         task control blocks (TCBs) are already allocated for each task, and it
*         populates the stacks with necessary initialization values, such as registers
*         and the program counter (PC). The function iterates through all tasks, setting
*         the stack with a dummy xPSR value, the task handler function address, and initial
*         values for the registers.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note
*         - The function assumes that `psp_of_tasks` is an array of pointers to
*           task-specific stack pointers and that `task_handlers` is an array of
*           function pointers for each task handler.
*         - `NUMBEROFTASK` should reflect the total number of tasks in the system.
*         - Ensure that all task stacks are properly aligned to the required boundary
*           for ARM processors.
*@return     None
********************************************************************************/
void init_tasks_stack (void)
{

	user_tasks[IDLETASK].current_state 	= TASK_READY_STATE;
	user_tasks[TASK1].current_state 	= TASK_READY_STATE;
	user_tasks[TASK2].current_state 	= TASK_READY_STATE;
	user_tasks[TASK3].current_state 	= TASK_READY_STATE;
	user_tasks[TASK4].current_state 	= TASK_READY_STATE;

	user_tasks[IDLETASK].psp_value 		= IDLE_STACK_START;
	user_tasks[TASK1].psp_value 		= T1_STACK_START;
	user_tasks[TASK2].psp_value 		= T2_STACK_START;
	user_tasks[TASK3].psp_value 		= T3_STACK_START;
	user_tasks[TASK4].psp_value 		= T4_STACK_START;

	user_tasks[IDLETASK].task_handler 	= idle_task;
	user_tasks[TASK1].task_handler 		= task1_handler;
	user_tasks[TASK2].task_handler 		= task2_handler;
	user_tasks[TASK3].task_handler 		= task3_handler;
	user_tasks[TASK4].task_handler 		= task4_handler;

	uint32 *pPSP;

	for(uint8 TaskIndex = IDLETASK; TaskIndex < NUMBEROFTASK; TaskIndex++ )
	{
		pPSP = (uint32 *)user_tasks[TaskIndex].psp_value;

		pPSP--; /*! xPSR */
		*pPSP = DUMMY_XPSR; /*! 0x01000000 */

		pPSP--; /*! PC */
		*pPSP = (uint32) user_tasks[TaskIndex].task_handler;

		pPSP--; /*! LR */
		*pPSP = LR_PSP_TO_PSP;

		for(uint8 InstIndex = R12; InstIndex < NUMBEROFINST; InstIndex++ )
		{
			pPSP--; /*! R12-R3-R2-R1-R0-R11-R10-R9-R8-R7-R6-R5-R4 */
			*pPSP = INITIAL_ZERO;
		}

		user_tasks[TaskIndex].psp_value = (uint32) pPSP;
	}
}

/********************************************************************************
*@brief   Enable processor fault handling exceptions.
*@details
*         This function configures the processor to handle specific fault exceptions,
*         including memory management faults, bus faults, and usage faults. By writing
*         to the System Control Space (SCS) register, it enables the corresponding exception
*         lines for each type of fault.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note
*         - This function accesses the SHCSR (System Handler Control and State Register)
*           in the SCS to enable fault exceptions.
*         - Enabling these faults ensures the processor will trigger exceptions on memory
*           violations, illegal instructions, or bus errors, depending on the configuration.
*         - Be cautious when enabling faults, as it may interfere with normal system behavior.
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
*@brief   Get the current task's Process Stack Pointer (PSP) value.
*@details
*         This function retrieves the Process Stack Pointer (PSP) for the currently
*         running task. It returns the PSP value, which is stored in the `psp_of_tasks`
*         array at the index corresponding to the current task.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note
*         - Ensure that the global variable `current_task` is properly managed to
*           reflect the currently executing task.
*         - The function assumes that `psp_of_tasks` is an array where each element
*           holds the PSP of the corresponding task.
*@return     The PSP value of the currently running task.
********************************************************************************/
uint32 get_psp_value (void)
{

	return user_tasks[current_task].psp_value;
}

/********************************************************************************
*@brief   Save the current task's Process Stack Pointer (PSP) value.
*@details
*         This function stores the provided PSP value (`current_psp_value`) for the
*         currently running task. The PSP is saved in the `psp_of_tasks` array at
*         the index corresponding to the current task. This allows for future retrieval
*         of the PSP when switching tasks.
*********************************************************************************
*@param[in]  current_psp_value: The PSP value to be saved for the current task.
*                                 This value will be stored in the `psp_of_tasks` array.
*@param[out] None
*@note
*         - Ensure that the `current_task` global variable correctly tracks the
*           currently executing task.
*         - The function assumes that `psp_of_tasks` is properly allocated and indexed
*           according to tasks.
*@return     None
********************************************************************************/
void save_psp_value (uint32 current_psp_value)
{
	user_tasks[current_task].psp_value = current_psp_value;
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
void update_next_task (void)
{
  current_state_t state = TASK_BLOCKED_STATE;

  for (uint8 TaskIndex= IDLETASK; TaskIndex < NUMBEROFTASK; TaskIndex++)
  {
	current_task++;
	current_task %= NUMBEROFTASK;
	state = user_tasks[current_task].current_state;
	if( (TASK_READY_STATE == state ) && (IDLETASK != current_task) )
	  break;
  }
  if(state != TASK_READY_STATE)
  {
	current_task = IDLETASK;
  }
}

/********************************************************************************
*@brief   Switch the stack pointer from Main Stack Pointer (MSP) to Process Stack Pointer (PSP).
*@details
*         This function switches the stack pointer used by the processor from the main
*         stack pointer (MSP) to the process stack pointer (PSP). It initializes the
*         PSP with the current task’s stack address and updates the CONTROL register to
*         indicate the use of PSP. This function is crucial in a task-switching mechanism
*         to handle multiple tasks using individual stack spaces.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note
*         - This function is marked as naked, meaning it does not have prologue or epilogue
*           generated by the compiler.
*         - The processor's CONTROL register is modified, so the execution mode of the processor
*           changes to use PSP for task execution.
*         - This function ensures the transition of stack pointer when switching tasks in a multi-tasking system.
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
void schedule(void)
{
  /*! pend the pendsv exception */
  uint32 *pICSR = (uint32 *) 0xE000ED04;
  *pICSR |= (1 << 28);

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
void task_delay(uint32 tick_count)
{
  /*! disable interrupt (it will be race condition because this variable (usertasks) is global variable.) */
  INTERRUPT_DISABLE();

  if(IDLETASK != current_task)
  {
	user_tasks[current_task].block_count = g_tick_count + tick_count;
	user_tasks[current_task].current_state = TASK_BLOCKED_STATE;
	schedule();
  }

   /*! enable interrupt (it will be race condition because this variable (usertasks) is global variable.) */
  INTERRUPT_ENABLE();

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
__attribute__((naked)) void PendSV_Handler(void)
{
	/*! Save the context of current task */
	/*! 1.Get current running task's PSP value */
	__asm volatile ("MRS R0, PSP");

	/*! 2.Using that PSP value store SF2(R4 to R11) */
	__asm volatile ("STMDB R0!, {R4-R11}");

	__asm volatile ("PUSH {LR}");
	/*! 3.Save the current value of PSP */
	__asm volatile ("BL save_psp_value");



	/*! Retrieve the context of next task */
	/*! 1.Decide next task to run */
	__asm volatile ("BL update_next_task");

	/*! 2.Get its past PSP value */
	__asm volatile ("BL get_psp_value");

	/*! 3.Using that PSP value retrieve SF2(R4 to R11) */
	__asm volatile ("LDMIA R0!, {R4-R11}");
	/*! 4.Update PSP and exit */
	__asm volatile ("MSR PSP, R0");
	__asm volatile ("POP {LR}");
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
void update_global_tick_count(void)
{
  g_tick_count++;
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
void unblock_tasks(void)
{
  for (uint8 TaskIndex= TASK1; TaskIndex < NUMBEROFTASK; TaskIndex++)
  {
	if(user_tasks[TaskIndex].current_state != TASK_READY_STATE)
	{
	  if(user_tasks[TaskIndex].block_count == g_tick_count)
	  {
		user_tasks[TaskIndex].current_state = TASK_READY_STATE;
	  }
	}
  }
}

/********************************************************************************
*@brief   SysTick interrupt handler for context switching between tasks.
*@details
*         This function is the SysTick interrupt handler, which is called periodically
*         at the frequency defined by the SysTick timer. It performs the necessary steps
*         to save the context of the current task, select the next task, and restore the
*         context of the new task for execution. It facilitates the implementation of
*         a round-robin scheduling system in a multitasking environment.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note
*         - This function operates within the SysTick interrupt and assumes the SysTick
*           interrupt is configured with a high enough frequency to trigger periodic task
*           switching.
*         - The use of PSP (Process Stack Pointer) ensures each task has its own stack,
*           maintaining separation between tasks.
*         - Context switching is done by storing and restoring register values (R4-R11)
*           of the currently running task and the task to be switched to.
*@return     None
********************************************************************************/
void SysTick_Handler (void)
{

  uint32 *pICSR = (uint32 *) 0xE000ED04;

  update_global_tick_count();

  unblock_tasks();
  /*! pend the pendsv exception */
  *pICSR |= (1 << 28);

}

/********************************************************************************
*@brief   HardFault interrupt handler.
*@details
*         This function is the HardFault interrupt handler, which is triggered when
*         the processor encounters a HardFault exception. This is usually caused by
*         an error that is not recoverable within the program, such as accessing invalid
*         memory addresses or other serious faults. The handler prints an error message
*         and then enters an infinite loop, effectively halting further execution.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note
*         - The HardFault handler is generally used to trap the program when an irrecoverable
*           error occurs. This allows for debugging or logging of critical issues before
*           the program halts.
*         - Typically, the processor halts within this handler, though more advanced applications
*           could perform diagnostics or error logging.
*@return     None
********************************************************************************/
void HardFault_Handler(void)
{
	printf("Exception : HardFault\n");
	while (1);
}

/********************************************************************************
*@brief   Memory Management exception handler.
*@details
*         This function is the MemManage interrupt handler, which is triggered when
*         a Memory Management exception occurs, typically due to access to illegal
*         memory regions (such as invalid pointer dereferencing) or violation of memory
*         protection settings. Upon detection of such a fault, the handler prints
*         an error message and halts execution by entering an infinite loop.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note
*         - This handler is part of the system’s error detection and recovery mechanism.
*           While this handler halts the execution by default, in production systems
*           more advanced error handling or logging mechanisms could be added for diagnostics.
*         - The handler provides a way to quickly identify and respond to memory errors.
*@return     None
********************************************************************************/
void MemManage_Handler(void)
{
	printf("Exception : MemManage\n");
	while (1);

}

/********************************************************************************
*@brief   BusFault exception handler.
*@details
*         This function is the BusFault interrupt handler, which is triggered when
*         a BusFault exception occurs. This typically happens in cases of access to
*         unavailable or invalid memory, misaligned data access, or an unsuccessful
*         peripheral access. When such an exception is raised, the handler prints
*         an error message and halts execution by entering an infinite loop.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note
*         - BusFaults are typically caused by hardware issues like incorrect memory
*           access or invalid instruction fetch. The handler prints an error message
*           and halts the program, but could also be extended to provide additional
*           diagnostic functionality.
*         - It’s a good practice to diagnose the root cause of the BusFault in more
*           advanced systems or for production usage.
*@return     None
********************************************************************************/
void BusFault_Handler(void)
{
	printf("Exception : BusFault\n");
	while (1);

}

/*! @} */
