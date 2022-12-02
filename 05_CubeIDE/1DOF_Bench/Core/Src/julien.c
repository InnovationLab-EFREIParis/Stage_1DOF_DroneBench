/*
 * julien.c
 *
 *  Created on: Nov 29, 2022
 *      Author: Julien
 */

#include "julien.h"

void landing_func(double landing_value, TIM_HandleTypeDef htim3,
		int valeur_min_moteur) {
	do {
		landing_value--;
		load_pwm(htim3, landing_value);
		HAL_Delay(25);
	} while (landing_value > valeur_min_moteur);
}

// PID coefficients

void reinit_pid_values(char r_buffer_string_k[], int *value_k0, int *value_k1,
		int *value_k2, int *value_k3, int *cpt_char_k) {
	r_buffer_string_k[0] = 0;
	r_buffer_string_k[1] = 0;
	r_buffer_string_k[2] = 0;
	r_buffer_string_k[3] = 0;
	*value_k0 = 0;
	*value_k1 = 0;
	*value_k2 = 0;
	*value_k3 = 0;
	*cpt_char_k = 0;
}