#include "stm32f4xx_it.h"
#include <stm32f4xx_hal_can.h>

#include "Can/can.h"
#include <stdbool.h>
#include "leds/leds.h"

extern CAN_HandleTypeDef hcan1;

void NMI_Handler(void) {
	while (1) {
	}
}

void HardFault_Handler(void) {
	while (1) {
	}
}

void MemManage_Handler(void) {
	while (1) {
	}
}

void BusFault_Handler(void) {
	while (1) {
	}
}

void UsageFault_Handler(void) {
	while (1) {
	}
}

void SVC_Handler(void) {
}

void DebugMon_Handler(void) {
}

void PendSV_Handler(void) {
}

void SysTick_Handler(void) {
	static int work_led_cnt = 0u;
	static bool work_led_state = false;
	HAL_IncTick();

	if (work_led_state && work_led_cnt >= 100u) {
		work_led_cnt = 0u;
		work_led_state = false;
		Leds_turnOff(LED_1);
	}
	if (!work_led_state && work_led_cnt >= 400u) {
		work_led_cnt = 0u;
		work_led_state = true;
		Leds_turnOn(LED_1);
	}
	work_led_cnt++;

}

void CAN1_TX_IRQHandler(void) {
	HAL_CAN_IRQHandler(&hcan1);
}

void CAN1_RX0_IRQHandler(void) {
	HAL_CAN_IRQHandler(&hcan1);
}

void CAN1_RX1_IRQHandler(void) {
	HAL_CAN_IRQHandler(&hcan1);
}

