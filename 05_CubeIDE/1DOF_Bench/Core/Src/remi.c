/*
 * remi.c
 *
 *  Created on: Feb 7, 2022
 *      Author: Remi
 */

#include "remi.h"

// On board green LED On
void setGreenLed() {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, SET);
}

// On board green LED blink
void blinkGreenLed(int nb, int delay) {
	for (int i = 0; i < nb; i++) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		HAL_Delay(delay);
	}
}

// set green led if blue push button is pressed
void setGreenLedViaButton() {
	// Gpio, push button
	// by default, PUSH_BUTTON is at 1
	//PinState = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 1)
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, RESET);
	else
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, SET);

}

// Welcome Message On RS232
void sendWelcomeMsgRS232(UART_HandleTypeDef *huart) {
	//printf("Hello from remi\n\r");
	//pointer targets in passing argument 2 of 'HAL_UART_Transmit' differ in signedness [-Wpointer-sign]
	//if (HAL_UART_Transmit(huart, "Nucleo L476RG connected\n\r", 25, 100)
	//modification au niveau du formatage du message
	if (HAL_UART_Transmit(huart, (uint8_t*)"Nucleo L476RG connected\n\r", 25, 100)
			!= HAL_OK)
		Error_Handler();
}

// Welcome Message On RS232
void sendbackRS232(UART_HandleTypeDef *huart) {
	if (HAL_UART_Receive(huart, rx_buffer, 1, 10) == HAL_OK) {
		HAL_UART_Transmit(huart, rx_buffer, 1, 10);
	} else {
		__HAL_UART_CLEAR_OREFLAG(huart);
	}
}

