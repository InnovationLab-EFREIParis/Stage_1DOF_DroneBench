/*
 * remi.h
 *
 *  Created on: Feb 7, 2022
 *      Author: Remi
 */

#ifndef INC_REMI_H_
#define INC_REMI_H_
#include "main.h"
#include  <stdio.h>
#include  <errno.h>
#include  <sys/unistd.h>


void setGreenLed();

void blinkGreenLed(int nb, int delay);
void sendWelcomeMsgRS232(UART_HandleTypeDef *huart);
void sendbackRS232(UART_HandleTypeDef *huart);
void setGreenLedViaButton();

#endif /* INC_REMI_H_ */
