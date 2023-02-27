/*
 * julien.h
 *
 *  Created on: Nov 29, 2022
 *      Author: Julien
 */

#ifndef INC_JULIEN_H_
#define INC_JULIEN_H_
#include "tim.h"
#include "yann.h"

enum choice_mode{choice_auto, choice_trip};

void landing_func(double landing_value, TIM_HandleTypeDef htim3, int valeur_min_moteur);
void reinit_pid_values(char r_buffer_string_k[], int *value_k0, int *value_k1, int *value_k2, int *value_k3, int *cpt_char_k);


#endif /* INC_JULIEN_H_ */
