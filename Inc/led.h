/*!**************************************************************************************************
 * @name : task_scheduler
 * @file : led.h
 * @date : Jan 8, 2025
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
#ifndef LED_H_
#define LED_H_
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
#define LED_GREEN  12
#define LED_ORANGE 13
#define LED_RED    14
#define LED_BLUE   15

#define DELAY_COUNT_1MS 		 1250U
#define DELAY_COUNT_1S  		(1000U * DELAY_COUNT_1MS)
#define DELAY_COUNT_500MS  		(500U  * DELAY_COUNT_1MS)
#define DELAY_COUNT_250MS 		(250U  * DELAY_COUNT_1MS)
#define DELAY_COUNT_125MS 		(125U  * DELAY_COUNT_1MS)

#define DELAY_1S  				1000
#define DELAY_500MS  			500
#define DELAY_250MS 			250
#define DELAY_125MS 			125
/*==================================================================================================
 *                                             ENUMS
 ==================================================================================================*/
/*==================================================================================================
 *                                 STRUCTURES AND OTHER TYPEDEFS
 ==================================================================================================*/
/*==================================================================================================
 *                                 GLOBAL VARIABLE DECLARATIONS
 ==================================================================================================*/
/*==================================================================================================
 *                                     FUNCTION PROTOTYPES
 ==================================================================================================*/
void led_init_all(void);
void led_on(uint8_t led_no);
void led_off(uint8_t led_no);
void delay(uint32_t count);





#endif /* LED_H_ */
