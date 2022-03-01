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
#include"fsm.h"
//SOUCI : ../Core/Inc/yann.h:13:15: error: unknown type name 'TIM_HandleTypeDef'
int load_adc(ADC_HandleTypeDef hadc,int polTime); //this function will start the adc and return values
void load_pwm(TIM_HandleTypeDef htimX,int val);
void y_print(UART_HandleTypeDef *huart,char *mess,int len);
void display_state(enum states etat,UART_HandleTypeDef *huart);
void chating_uart(UART_HandleTypeDef *huart,char *buffer);

<<<<<<< HEAD
float firmware_version = 0.1;
int Valeur_minimale_moteur =1512;

=======
float firmware_version=0.1;
>>>>>>> refs/remotes/origin/main
#endif /* INC_YANN_H_ */
