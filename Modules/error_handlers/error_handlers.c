/**
 ******************************************************************************
 * @file           : error_handlers.c
 * @author         : Jacek Prokopczuk, Krystian Czechowicz, Adam Rybojad
 * @brief          : Handling all errors that is detected TODO
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------- */

#include <stm32f4xx_hal.h>
#include "error_handlers/error_handlers.h"
#include "leds/leds.h"

#include "Can/can.h"

uint8_t ERROR_COUNT = 0;
bool ERROR_ACTIVE = 0;
uint8_t ERROR_CURRENT = 0;

/* Functions ------------------------------------------------------------------- */

/**
 * @see  documentation in the header file (error_handlers.h)
 */
void Error_Handler(Error_function error_func, Error_code error_code) {
//	__disable_irq();

	ERROR_ACTIVE = 1;

	if (ERROR_COUNT > 3) {
		while (1) {
		}
	}

	if (ERROR_CURRENT == error_func) {
		ERROR_COUNT++;
		return;
	}

	if (ERROR_CURRENT != 0) {
		// we have two different errors
		while (1) {
		}
	}

	ERROR_CURRENT = error_func;

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
		while (1) {
		}
	}
	ERROR_COUNT = 0;
	ERROR_CURRENT = 0;

//	__enable_irq();
}

