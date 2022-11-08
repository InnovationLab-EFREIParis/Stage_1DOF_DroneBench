/*
 * yann.c
 *
 *  Created on: 7 févr. 2022
 *      Author: AXEL NDO
 */

#include "yann.h"
#include "tim.h"

float firmware_version = 0.3;
int valeur_min_moteur = max_period_timer / 2 - 225; //1512;
// htim3.Init.Period-valeur_min_moteur-10
//4096 - 1512 - 10 =
int valeur_max_moteur = max_period_timer; // htim3.Init.Period//2584;

int somme = 0;
int moy = 0;
float remap;
int map;
int concat_value;
char *tampon;
int iter = 0;

/*int concat(char *tableau){
 while(tableau[iter]!='\n')
 iter++;
 tampon = (char*)malloc(iter*sizeof(char));

 for (int i = 0; i < iter; ++i) {
 tampon[iter]= tableau[iter];
 }
 concat_value=(int) tampon;
 printf("%d",concat_value);
 iter=0;

 free(tampon);
 return concat_value;
 }*/

int load_adc(ADC_HandleTypeDef hadc, int polTime) {

	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, polTime);
	return HAL_ADC_GetValue(&hadc);

}

int mapping_adc_value(int val) {
	//remap = 1512 + (val * 2583 / 4095);
	//remap = valeur_min_moteur + (val * valeur_max_moteur / 4095);
	//int map = remap;
	return (valeur_min_moteur + (val * valeur_max_moteur) / 4095);
}

int mapping_adc_value_percent(int val) {

	return (valeur_min_moteur + (val * (valeur_max_moteur) / 100));
}

void load_pwm(TIM_HandleTypeDef htimX, int val) {

	htimX.Instance->CCR2 = val;
}




