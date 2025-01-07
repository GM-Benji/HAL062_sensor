/**
 ******************************************************************************
 * @file           : error_handlers.c
 * @author         : Jacek Prokopczuk, Krystian Czechowicz, Adam Rybojad
 * @brief          : Handling all errors that is detected TODO
 ******************************************************************************
 */

/* Includes -------------------------------------------------------------------*/

#include <stm32f4xx_hal.h>
#include "error_handlers/error_handlers.h"
#include "leds/leds.h"
#include "Can/can.h"

static uint8_t ERROR_COUNT = 0;
static bool ERROR_ACTIVE = 0;
static uint8_t ERROR_CURRENT = 0;

/* Functions ------------------------------------------------------------------*/

static void critical_handle(Error_code error_code);

/**
 * @see  documentation in the header file (error_handlers.h)
 */
void Error_Handler(Error_function error_func, Error_code error_code) {
	__disable_irq();

	ERROR_ACTIVE = 1;

	if (ERROR_COUNT > 3) {
		critical_handle(error_code);
	}

	if (ERROR_CURRENT == error_func) {
		ERROR_COUNT++;
		return;
	}

	if (ERROR_CURRENT != 0) {
		// we have two different errors
		critical_handle(error_code);
	}

	ERROR_CURRENT = (uint8_t) error_func;

	if (error_func != CANErrorFunc_init)
		CAN_sendMessage((uint8_t*) &error_code, (uint8_t) BOARD_ERROR_ID);

	switch (error_func) {
	case ErrorFunc_test:
		ERROR_ACTIVE = 0;
		break;

	case CANErrorFunc_init:
		while (ERROR_ACTIVE) {
			extern CAN_HandleTypeDef hcan1;
			ERROR_ACTIVE = 0;

			HAL_CAN_MspDeInit(&hcan1);
			CAN_Init();
		}
		break;

	case SysClkErrorFunc_init:
		break;

	default:
		break;
	}

	if (ERROR_ACTIVE == 1) {
		critical_handle(error_code);
	}
	ERROR_COUNT = 0;
	ERROR_CURRENT = 0;

	__enable_irq();
}

/**
 ******************************************************************************
 * @brief	:	Reset or debug via leds in case of critical error  
 * 
 * @details	:	If DEBUG_LEDS is defined shows the error code whith the use of 
 * leds. Because we need more than 5 bits to display the error, we blink leds 
 * 1-3 for 100ms every second to display another 3 bits with led 1 beeng the 
 * least sugnificant. 
 *
 * If DEBUG_LEDS is not defined function resets the board
 ******************************************************************************
 */
[[noreturn]]
static void critical_handle(Error_code error_code) {
#ifdef DEBUG_LEDS	

	extern IWDG_HandleTypeDef hiwdg1;
	HAL_IWDG_Refresh(&hiwdg1);

	Leds_turnOff(LED_ALL);

	if (error_code & 0b00000001) {Leds_turnOn(LED_1);}
	if (error_code & 0b00000010) {Leds_turnOn(LED_2);}
	if (error_code & 0b00000100) {Leds_turnOn(LED_3);}
	if (error_code & 0b00001000) {Leds_turnOn(LED_4);}

	uint16_t owerflow_leds = 0;
	if (error_code & 0b00010000) {owerflow_leds |= LED_1;}
	if (error_code & 0b00100000) {owerflow_leds |= LED_2;}
	if (error_code & 0b01000000) {owerflow_leds |= LED_3;}
	if (error_code & 0b10000000) {owerflow_leds |= LED_4;}

	while (1) {
		HAL_IWDG_Refresh(&hiwdg1);
		HAL_Delay(1000);

		// blink leds for a short time to get more precision
		Leds_toggle(owerflow_leds);
		HAL_Delay(100);
		Leds_toggle(owerflow_leds);
	}

#else

	NVIC_SystemReset();

#endif
}
