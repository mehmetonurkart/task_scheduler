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

void idle_task(void); 	  /*!  This is task idle */
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
*         This function is the entry point for the system. It sets up the processor
*         faults, initializes the scheduler stack, task stacks, and peripherals.
*         The SysTick timer is configured to handle task scheduling, and the
*         processor stack pointer is switched to the process stack pointer (PSP).
*         Finally, Task 1 is started as the initial task, and the system enters an
*         infinite loop.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note
*         Ensure that all hardware peripherals and system initialization functions
*         are called before switching to the PSP and entering the task execution phase.
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
*@brief   Idle task for the system.
*@details
*         The idle task runs when no other tasks are ready to execute. It is an
*         essential component of the scheduler, ensuring that the CPU is not left
*         idle or in an unknown state.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note
*         - The idle task must have the lowest priority in the system.
*         - It can be extended to include power-saving features such as entering
*           low-power modes when the CPU is not required.
*@return     None
********************************************************************************/
void idle_task(void)
{
  while(1);
}

/********************************************************************************
*@brief   Handles Task 1 functionality.
*@details
*         Task 1 toggles the state of a green LED in 1-second intervals.
*         The task turns the LED on, waits for a delay of 1 second, and then
*         turns it off, repeating this behavior indefinitely.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note
*         - The `task_delay` function ensures that the task scheduler handles the
*           timing, allowing other tasks to execute during the delay period.
*         - Ensure `LED_GREEN` is defined, and the `led_on`, `led_off`, and
*           `task_delay` functions are properly implemented.
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
*@brief   Handles Task 2 functionality.
*@details
*         Task 2 toggles the state of an orange LED in 500-millisecond intervals.
*         The task turns the LED on, waits for a delay of 500 milliseconds, and
*         then turns it off, repeating this behavior indefinitely.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note
*         - The `task_delay` function ensures efficient time management by allowing
*           other tasks to run during the delay period.
*         - Ensure `LED_ORANGE` is defined, and the `led_on`, `led_off`, and
*           `task_delay` functions are properly implemented and functional.
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
*@brief   Handles Task 3 functionality.
*@details
*         Task 3 toggles the state of a blue LED in 250-millisecond intervals.
*         The task turns the LED on, waits for a delay of 250 milliseconds, and
*         then turns it off, repeating this behavior indefinitely.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note
*         - The `task_delay` function helps other tasks run during the delay,
*           making the system more efficient and responsive.
*         - Ensure `LED_BLUE` is properly defined, and the `led_on`, `led_off`,
*           and `task_delay` functions are properly implemented.
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
*@brief   Handles Task 4 functionality.
*@details
*         Task 4 toggles the state of a red LED in 125-millisecond intervals.
*         The task turns the LED on, waits for a delay of 125 milliseconds, and
*         then turns it off, repeating this behavior indefinitely.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note
*         - The `task_delay` function provides a time-based delay, enabling
*           cooperative multitasking and ensuring other tasks can execute during
*           this period.
*         - Ensure `LED_RED` is defined, and the `led_on`, `led_off`, and
*           `task_delay` functions are implemented correctly.
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
*@brief   Initializes the SysTick timer.
*@details
*         This function sets up the SysTick timer to generate periodic interrupts
*         based on the desired tick frequency (`tick_hz`). It configures the
*         SysTick Reload Value Register (SRVR) with the correct tick count, sets
*         the clock source to the processor's clock, and enables the SysTick
*         exception request.
*********************************************************************************
*@param[in]  tick_hz     The desired SysTick interrupt frequency in Hertz (ticks per second).
*@param[out] None
*@note
*         - Make sure that `SYSTICK_TIM_CLK` and `RELOAD_VALUE_OFFSET` are defined appropriately.
*         - The SysTick timer is configured to generate periodic interrupts at the specified frequency.
*         - SysTick exception handling must be implemented to ensure it works as expected.
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
*@brief   Initializes the task stack and sets up task control blocks (TCB).
*@details
*         This function initializes the stacks for each task, including the idle task and user-defined tasks (TASK1, TASK2, TASK3, TASK4).
*         It sets up the `current_state`, `psp_value`, and `task_handler` for each task. The task stacks are initialized with the
*         proper values for the initial state, program counter, link register, and registers.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note
*         - Ensure that stack memory (`T1_STACK_START`, `T2_STACK_START`, etc.) is correctly allocated before calling this function.
*         - The task control blocks (`user_tasks[]`) should be initialized properly.
*         - This function ensures that when a task is scheduled, it will resume execution from the correct state in its stack.
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
*@brief   Returns the current PSP value for the running task.
*@details
*         This function fetches the current Process Stack Pointer (PSP) value
*         for the task that is currently running, based on the `current_task`.
*         The function returns the address where the current task's stack frame is stored.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note       Ensure that `current_task` has been properly updated before calling this function.
*@return     uint32    The current PSP value of the running task (the stack pointer for the task).
********************************************************************************/
uint32 get_psp_value (void)
{

	return user_tasks[current_task].psp_value;
}

/********************************************************************************
*@brief   Saves the current PSP value of the running task.
*@details
*         This function stores the provided PSP value for the currently running task
*         in the `user_tasks` array, ensuring that the task’s stack pointer is saved for
*         later context restoration.
*********************************************************************************
*@param[in]  current_psp_value    The current Process Stack Pointer (PSP) value to be saved.
*@param[out] None
*@note       This function is typically called before context switching to save the stack
*            pointer of the current task.
*@return     None
********************************************************************************/
void save_psp_value (uint32 current_psp_value)
{
	user_tasks[current_task].psp_value = current_psp_value;
}

/********************************************************************************
*@brief   Updates the task that will run next.
*@details
*         This function scans through the tasks and selects the next task that is in
*         the READY state to be executed. If no tasks are in the READY state, the IDLE task
*         will be selected by default.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note       This function is used for selecting which task should run next based on
*            the state of the tasks (READY or BLOCKED). The current task pointer (`current_task`)
*            is updated accordingly. If no READY tasks are available, the IDLE task is selected.
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
*@brief   Schedules the next task to run.
*@details
*         This function triggers a PendSV exception to switch to the next task in
*         a round-robin manner. PendSV is a special exception used for context switching.
*         This function does not directly perform the context switch; it just requests
*         it by setting a specific bit in the Interrupt Control and State Register (ICSR).
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note       This function must be called at an appropriate moment during your
*            RTOS (Real-Time Operating System) to initiate a context switch between tasks.
*@return     None
********************************************************************************/
void schedule(void)
{
  /*! pend the pendsv exception */
  uint32 *pICSR = (uint32 *) 0xE000ED04;
  *pICSR |= (1 << 28);

}

/********************************************************************************
*@brief   Delays the current task for a specified number of system ticks.
*@details
*         This function blocks the current task for the given number of system ticks.
*         The `block_count` of the task is updated and the task is moved to the
*         `TASK_BLOCKED_STATE`. The scheduler is called after updating the task state,
*         causing the next task to execute.
*         The function uses an interrupt disabling mechanism to prevent a race condition
*         while modifying the global task list.
*********************************************************************************
*@param[in]  tick_count - The number of system ticks to delay the task.
*@param[out] None
*@note       The task is blocked and a context switch occurs. After the specified delay,
*            the task will be unblocked when the global tick count exceeds the `block_count`.
*            This implementation uses a non-blocking mechanism for delay handling.
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
*@brief   PendSV Exception Handler for task context switching.
*@details
*         This handler is responsible for saving the context of the currently executing
*         task and restoring the context of the next task to run. It is invoked in response
*         to a PendSV exception, typically triggered by a context switch request (i.e. scheduling).
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note       This handler performs a full context save and restore, allowing a switch
*            from one task to another. It manages the task's register state to ensure
*            proper multitasking operation.
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
*@brief   Updates the global tick counter.
*@details This function increments the global tick count variable, which keeps track of the system's tick time.
           It should be called periodically, typically by a timer interrupt, to ensure proper timing in the task scheduler.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note       This function is crucial for task management and scheduling in a real-time operating system (RTOS) framework.
*@return     None
********************************************************************************/
void update_global_tick_count(void)
{
  g_tick_count++;
}

/********************************************************************************
*@brief   Unblocks tasks that have finished their delay period.
*@details This function checks all tasks that are blocked and compares their block count with the global tick count.
           If the block count matches the current global tick count, the task is unblocked by setting its state to READY.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note       This function is typically called within the system tick handler to periodically check and unblock tasks
            that are waiting for their delay to expire.
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
*@brief   Handles the SysTick interrupt to update the global tick count and unblock tasks.
*@details This function is triggered at regular intervals by the SysTick timer. It updates the global tick count
           and checks which tasks have completed their delay period. If a task's delay period has ended, it
           unblocks the task. It then triggers a PendSV exception to switch the context to the next task.
*********************************************************************************
*@param[in]  None
*@param[out] None
*@note       This function is called automatically by the interrupt vector table when the SysTick timer overflows.
            It is used for time-based task scheduling and context switching.
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
