/*
 * julien.c
 *
 *  Created on: Nov 29, 2022
 *      Author: Julien
 */

#include "julien.h"

void landing_func(double landing_value, TIM_HandleTypeDef htim3, int valeur_min_moteur) {
	do {
		landing_value--;
		load_pwm(htim3, landing_value);
		HAL_Delay(25);
	} while (landing_value > valeur_min_moteur);
}
