/*
 * yann.c
 *
 *  Created on: 7 févr. 2022
 *      Author: AXEL NDO
 */


#include "yann.h"
#include<stdio.h>


 float firm =0.1;

int load_adc(ADC_HandleTypeDef hadc, int polTime) {

	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, polTime);
	//if(HAL_ADC_GetValue(&hadc)==0)
	//return 1;
	return HAL_ADC_GetValue(&hadc);

}

void load_pwm(TIM_HandleTypeDef htimX, int val) {

	htimX.Instance->CCR2 = val;
}

void y_print(UART_HandleTypeDef *huart, char *mess,int len) {

	if (HAL_UART_Transmit(huart, (uint8_t*) mess,len, 100) != HAL_OK)
		Error_Handler();
}


void display_state(enum states etat, UART_HandleTypeDef *huart) {
	switch (etat) {


	case info_mode:
		if (HAL_UART_Transmit(huart, (uint8_t*) "Info mode\n\r", 12, 100)
				!= HAL_OK)
			Error_Handler();
		HAL_Delay(3000);

		break;
	case init_uc:
		if (HAL_UART_Transmit(huart, (uint8_t*) "UC Initialization \n\r", 22,
				100) != HAL_OK)
			Error_Handler();
		HAL_Delay(3000);
		break;
	case init_motor:
		if (HAL_UART_Transmit(huart, (uint8_t*) "Motor Initialization \n\r", 24,
				100) != HAL_OK)
			Error_Handler();
		HAL_Delay(3000);
		break;
	case motor_ready:
		if (HAL_UART_Transmit(huart, (uint8_t*) "Motor ready \n\r", 15, 100)
				!= HAL_OK)
			Error_Handler();
		HAL_Delay(3000);

		break;
	case auto_mode:
		if (HAL_UART_Transmit(huart, (uint8_t*) "Auto mode \n\r", 15, 100)
				!= HAL_OK)
			Error_Handler();
		HAL_Delay(3000);

		break;
	case manual_mode:
		if (HAL_UART_Transmit(huart, (uint8_t*) "Manual mode \n\r", 15, 100)
				!= HAL_OK)
			Error_Handler();
		HAL_Delay(3000);

		break;
	default:
		break;

	}

}
