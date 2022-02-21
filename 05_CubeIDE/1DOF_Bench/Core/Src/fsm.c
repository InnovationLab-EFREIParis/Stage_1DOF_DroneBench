/*
 * fsm.c
 *
 *  Created on: Feb 11, 2022
 *      Author: AXEL NDO
 */

#include "fsm.h"
#include "main.h"
#include "yann.h"




void switch_states(enum states div);

void motor_init() {


	HAL_Delay(1000);
	TIM3->CCR2 = 1512;
	HAL_Delay(5000);

}

void uc_init() {


	}


void ready_motor() {
		//what to do when motor ready?

			HAL_Delay(1000);
		}



void manual_m() {

				}


void auto_m() {

				}

void info() {

				}

