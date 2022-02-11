/*
 * fsm.h
 *
 *  Created on: Feb 11, 2022
 *      Author: AXEL NDO
 */
#include<stdio.h>
#ifndef INC_FSM_H_
#define INC_FSM_H_

enum states{init_uc,init_motor,motor_ready,manual_mode,auto_mode,info_mode};

void motor_init();
void uc_init();
void motor_ready();
void manual_m();
void auto_m();
void info();







#endif /* INC_FSM_H_ */
