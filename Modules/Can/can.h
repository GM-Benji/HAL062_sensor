
#ifndef MODULES_CAN_CAN_H
#define MODULES_CAN_CAN_H

#include <stm32f4xx_hal_can.h>
#include <stdbool.h>

void CAN_Init(void);

void CAN_testMessage(void);

void CAN_sendMessage(uint8_t* msg, uint8_t ID);

#endif // MODULES_CAN_CAN_H
