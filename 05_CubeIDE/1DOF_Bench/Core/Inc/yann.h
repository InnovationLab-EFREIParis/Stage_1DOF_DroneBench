/*
 * yann.h
 *
 *  Created on: 7 févr. 2022
 *      Author: AXEL NDO
 */
// in this part we are going to test the different pwm signals that we have to take into account
#ifndef INC_YANN_H_
#define INC_YANN_H_
#include"main.h"
//SOUCI : ../Core/Inc/yann.h:13:15: error: unknown type name 'TIM_HandleTypeDef'
int ADCinput(ADC_HandleTypeDef hadc,int polTime); //this function will start the adc and return values
void load_pwm(TIM_HandleTypeDef htimX,int val);




#endif /* INC_YANN_H_ */
