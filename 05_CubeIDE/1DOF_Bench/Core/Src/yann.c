/*
 * yann.c
 *
 *  Created on: 7 févr. 2022
 *      Author: AXEL NDO
 */

#include "yann.h"

float firmware_version = 0.1;
int valeur_min_moteur = 1512;
int valeur_max_moteur = 2025;
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

	return (valeur_min_moteur + (val * (valeur_max_moteur-valeur_min_moteur) / 100));
}

void load_pwm(TIM_HandleTypeDef htimX, int val) {

	htimX.Instance->CCR2 = val;
}

void y_print(UART_HandleTypeDef *huart, char *mess, int len) {

	if (HAL_UART_Transmit(huart, (uint8_t*) mess, len, 100) != HAL_OK)
		Error_Handler();
}

