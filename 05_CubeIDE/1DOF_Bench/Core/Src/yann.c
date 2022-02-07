/*
 * yann.c
 *
 *  Created on: 7 févr. 2022
 *      Author: AXEL NDO
 */

#include "yann.h"


int ADCinput(ADC_HandleTypeDef *hadc,int polTime){

	HAL_ADC_Start(hadc);
	HAL_ADC_PollForConversion(hadc, polTime);
	return  HAL_ADC_GetValue(hadc);

}

void load_pwm(TIM_HandleTypeDef *htimX,int val){

	htimX->Instance->CCR1=val ;
}


