/**
 ******************************************************************************
 * @file           : leds.c
 * @author         : J. Prokopczuk
 * @brief          : Leds driver
 ******************************************************************************
 */

#include <stm32f4xx_hal.h> // Simple HAL delay

#include "leds.h"

void Leds_init(void) {
	GPIO_InitTypeDef gpio;
	__HAL_RCC_GPIOC_CLK_ENABLE();
	gpio.Pin = LED_ALL;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_PULLDOWN;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_PORT, &gpio);
}

/*
 * ***************************************************
 * @ details	:	High level led turning on, using
 * 					only predefined macros
 * ***************************************************
 */
void Leds_turnOn(uint16_t ledId) {
	HAL_GPIO_WritePin(LED_PORT, ledId, GPIO_PIN_SET);
}

/*
 * ***************************************************
 * @ details	:	High level led turning off, using
 * 					only predefined macros
 * ***************************************************
 */
void Leds_turnOff(uint16_t ledId) {
	HAL_GPIO_WritePin(LED_PORT, ledId, GPIO_PIN_RESET);
}

/*
 * *****************************************************
 * @ details	:	Toggling LED using predefined macro
 * *****************************************************
 */
void Leds_toggle(uint16_t ledId) {
	HAL_GPIO_TogglePin(LED_PORT, ledId);
}

void Leds_welcomeFLash(void) {
	Leds_turnOn(LED_ALL);
	HAL_Delay(1000);
	Leds_turnOff(LED_ALL);
	HAL_Delay(1000);
}
