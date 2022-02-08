/*
 * remi.h
 *
 *  Created on: Feb 7, 2022
 *      Author: Remi
 */
#include "main.h"
#include <stdio.h>
#ifndef INC_REMI_H_
#define INC_REMI_H_

uint8_t rx_buffer[2];
GPIO_PinState PinState;


void setGreenLed();
void blinkGreenLed(int nb, int delay);
void sendWelcomeMsgRS232(UART_HandleTypeDef *huart);
void sendbackRS232(UART_HandleTypeDef *huart);
void setGreenLedViaButton();

#endif /* INC_REMI_H_ */
