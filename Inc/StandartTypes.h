/*!**************************************************************************************************
 * @Project : task_scheduler
 * @file : StandartTypes.h
 * @date : Dec 26, 2024
 * @author : mehmet.kart
 * @brief : None
 *
 * @Version  x.x
 *
 *---------------------------------------------------------------------------------------------------
 *
 * (c) Copyright xxxx-xxxx XXXXXXXX.
 * Copyright xxxx-xxxx XXXXXXXX.
 * All Rights Reserved.
 *
 ***************************************************************************************************/
#ifndef STANDARTTYPES_H_
#define STANDARTTYPES_H_
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

#define FALSE 0
#define TRUE  1

#define E_OK 	 0
#define E_NOT_OK 1

#define STD_LOW   0
#define STD_HIGH  1

#define STD_IDLE   0
#define STD_ACTIVE 1

/*==================================================================================================
 *                                             ENUMS
 ==================================================================================================*/
/*==================================================================================================
 *                                 STRUCTURES AND OTHER TYPEDEFS
 ==================================================================================================*/

/**
* @brief Unsigned 8 bit integer with range of 0 ..+255 (0x00..0xFF) -
*        8 bit
* @implements uint8_type
*/
typedef uint8_t uint8;

/**
* @brief Unsigned 16 bit integer with range of 0 ..+65535 (0x0000..0xFFFF) -
*        16 bit
* @implements uint16_type
*/
typedef uint16_t uint16;

/**
* @brief Unsigned 32 bit integer with range of 0 ..+4294967295 (0x00000000..0xFFFFFFFF) -
*        32 bit
* @implements uint32_type
*/
typedef uint32_t uint32;

/**
* @brief Unsigned 64 bit integer with range of 0..18446744073709551615 (0x0000000000000000..0xFFFFFFFFFFFFFFFF)-
*        64 bit
*
*/
typedef uint64_t uint64;


/**
* @brief Signed 8 bit integer with range of -128 ..+127 (0x80..0x7F) -
*        7 bit + 1 sign bit
* @implements sint8_type
*/
typedef int8_t sint8;

/**
* @brief Signed 16 bit integer with range of -32768 ..+32767 (0x8000..0x7FFF) -
*        15 bit + 1 sign bit
* @implements sint16_type
*/
typedef int16_t sint16;

/**
* @brief Signed 32 bit integer with range of -2147483648.. +2147483647 (0x80000000..0x7FFFFFFF) -
*        31 bit + 1 sign bit
* @implements sint32_type
*/
typedef int32_t sint32;

/**
* @brief Signed 64 bit integer with range of -9223372036854775808..9223372036854775807 (0x8000000000000000..0x7FFFFFFFFFFFFFFF )-
*        63 bit + 1 sign bit
*
*/
typedef int64_t sint64;

/**
* @brief Unsigned integer at least 8 bit long. Range of at least 0 ..+255 (0x00..0xFF) -
*        8 bit
* @implements uint8_least_type
*/
typedef uint_least8_t uint8_least;

/**
* @brief  Unsigned integer at least 16 bit long. Range of at least 0 ..+65535 (0x0000..0xFFFF) -
*         16 bit
* @implements uint16_least_type
*/
typedef uint_least16_t uint16_least;

/**
* @brief Unsigned integer at least 32 bit long. Range of at least 0 ..+4294967295
*       (0x00000000..0xFFFFFFFF) - 32 bit
* @implements uint32_least_type
*/
typedef uint_least32_t uint32_least;

/**
* @brief Signed integer at least 8 bit long. Range - at least -128 ..+127.
*        At least 7 bit + 1 bit sign
* @implements sint8_least_type
*/
typedef int_least8_t sint8_least;

/**
* @brief Signed integer at least 16 bit long. Range - at least -32768 ..+32767.
*        At least 15 bit + 1 bit sign
* @implements sint16_least_type
*/
typedef int_least16_t sint16_least;

/**
* @brief Signed integer at least 32 bit long. Range - at least -2147483648.. +2147483647.
*       At least 31 bit + 1 bit sign
* @implements sint32_least_type
*/
typedef int_least32_t sint32_least;

/**
* @brief 32bit long floating point data type
* @implements float32_type
*/
typedef float float32;

/**
* @brief 64bit long floating point data type
* @implements float64_type
*/
typedef double float64;

typedef uint8 Std_ReturnType;
/*==================================================================================================
 *                                 GLOBAL VARIABLE DECLARATIONS
 ==================================================================================================*/
/*==================================================================================================
 *                                     FUNCTION PROTOTYPES
 ==================================================================================================*/


#endif /* STANDARTTYPES_H_ */
