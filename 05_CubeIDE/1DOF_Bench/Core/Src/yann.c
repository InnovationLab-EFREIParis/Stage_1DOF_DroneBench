/*
 * yann.c
 *
 *  Created on: 7 févr. 2022
 *      Author: AXEL NDO
 */

#include "yann.h"



int load_adc(ADC_HandleTypeDef hadc,int polTime){

	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, polTime);
	//if(HAL_ADC_GetValue(&hadc)==0)
		//return 1;
	return  HAL_ADC_GetValue(&hadc);

}

void load_pwm(TIM_HandleTypeDef htimX,int val){

	htimX.Instance->CCR2=val ;
}

void y_print(UART_HandleTypeDef *huart,char *mess) {

	if (HAL_UART_Transmit(huart, (uint8_t*) mess, 25, 100)
			!= HAL_OK)
		Error_Handler();
}

