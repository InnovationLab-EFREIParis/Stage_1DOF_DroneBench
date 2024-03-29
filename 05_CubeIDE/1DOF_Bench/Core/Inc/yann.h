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
#include<stdlib.h>
#include<string.h>


enum states{entrance, init_uc, read_position, landing, motor_ready,manual_mode_pot,manual_mode_term,auto_mode, info_mode,init_gyro,instruct_angle,instruct_kp,instruct_ki,instruct_kd};

extern float firmware_version;
extern int  valeur_min_moteur;
extern int  valeur_max_moteur;

//SOUCI : ../Core/Inc/yann.h:13:15: error: unknown type name 'TIM_HandleTypeDef'
int load_adc(ADC_HandleTypeDef hadc,int polTime); //this function will start the adc and return values
void load_pwm(TIM_HandleTypeDef htimX,int val);
void y_print(UART_HandleTypeDef *huart,char *mess,int len);
void display_state(enum states etat,UART_HandleTypeDef *huart);
void chating_uart(UART_HandleTypeDef *huart,char *buffer);
void asservissement(float kp, float ki, float kd, int consigne, double position,  int _commande);

int mapping_adc_value(int val);
int mapping_adc_value_percent(int val);
int mapping_adc_value_permil(int val);
int concat(char *tableau);


#endif /* INC_YANN_H_ */
