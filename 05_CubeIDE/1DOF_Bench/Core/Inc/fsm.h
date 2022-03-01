/*
 * fsm.h
 *
 *  Created on: Feb 11, 2022
 *      Author: AXEL NDO
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include<stdio.h>
#include "main.h"
enum states{init_uc,init_motor,motor_ready,manual_mode,auto_mode,info_mode};



void switch_states(enum states div);
void motor_init();
void uc_init();
void ready_motor();
void manual_m();
void auto_m();
void info();







#endif /* INC_FSM_H_ */
