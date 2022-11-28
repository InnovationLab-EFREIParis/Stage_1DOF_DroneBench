/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/*
 * pinout :
 * pot : "3 pins{
 * 	vcc = 3.3v
 * 	signal = A0
 * 	gnd = gnd
 * }
 *
 * ESC : 2 pins{
 *  Signal = D11
 *  gnd = gnd
 * }
 *
 * Gyro : 4 pins{
 * 	vcc = 5v    **brancher sur 3.3
 * 	gnd=gnd
 * 	scl=PB8=d15
 * 	sda = PB9=d14
 *
 *
 * }
 *
 *
 *
 *
 * */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "yann.h"
#include "mpu6050.h"
#include  <stdio.h>
#include  <errno.h>
#include  <sys/unistd.h>
#include  <inttypes.h>
#include  <stdint.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
Kalman_t kali;
MPU6050_t mpu;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t gaz_buffer[4];

int _write(int file, char *data, int len) {
	if ((file != STDOUT_FILENO) && (file != STDERR_FILENO)) {
		errno = EBADF;
		return -1;
	}

	// arbitrary timeout 1000
	HAL_StatusTypeDef status = HAL_UART_Transmit(&huart2, (uint8_t*) data, len,
			1000);

	// return # of bytes written - as best we can tell
	return (status == HAL_OK ? len : 0);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	//uint16_t timer_val;
	uint32_t timer_val2=UINT32_C(1234567890);

	enum states etat, previous_etat;
	etat = entrance;
	char r_buffer[2];
	int okay;
	int valeur_can;
	int mapped_value = valeur_min_moteur;
	double position_angulaire;

	double commande = valeur_min_moteur;
	double landing_value = valeur_min_moteur;

	// Default coefficients
	double kp = 0.001;
	double ki = 0.018;
	double kd = 0.1;

	double erreur = 0;
	double _erreur = 0;
	double integre_erreur = 0;
	double derive_erreur = 0;

	// mode 2
	int cpt_char = 0;
	int max_cpt_char = 4;
	char r_buffer_string[max_cpt_char];
	int gaz_term_percent = 0;
	// mode 3
	int cpt_char_prime = 0;
	int max_cpt_char_prime = 3;
	char r_buffer_string_prime[max_cpt_char_prime];
	int angle_term = 0;
	int consigne = 0;

	int cpt_char_kp = 0;
	int max_cpt_char_kp = 5; //7 avec virgule et chiffre des unités
	char r_buffer_string_kp[max_cpt_char_kp];

	int cpt_char_ki = 0;
	int max_cpt_char_ki = 5; //7 avec virgule et chiffre des unités
	char r_buffer_string_ki[max_cpt_char_ki];

	int cpt_char_kd = 0;
	int max_cpt_char_kd = 5; //7 avec virgule et chiffre des unités
	char r_buffer_string_kd[max_cpt_char_kd];



	char msg_motor_ready[] = "> Press [ 0 ] or [ SPACE ] for Motor ready\n\r";
	char msg_info_mode[] = "> Press [ i ] for Info mode\n\n\r";
	char msg_error_char_nb[] = "ERROR: Please enter only characters included in this list : 0 1 2 3 4 5 6 7 8 9\n\n\r";
	char msg_error_value_sup[] = "ERROR: Value > 10\n\n\r";
	char msg_error_value_sup_angle[] = "ERROR: Value > 90\n\n\r";


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_DMA_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_TIM15_Init();
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */
//	while (MPU6050_Init(&hi2c1) == 1)
//		;
	// Light up green led
	//setGreenLed();
	// blink green led
	//HAL_UART_Receive_IT(&huart2, (uint8_t*)gaz_buffer, 4);
	//blinkGreenLed(10, 100);
	// Welcome message on UART
	//sendWelcomeMsgRS232(&huart2);
	//la fonction au dessus pose des soucis
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	//NOus mettons ici la valeur minimale pour emettre un signal vers notre ESC dans notre registre capture and compare register
	HAL_TIM_Base_Start(&htim15);
	HAL_TIM_Base_Start(&htim2);

	//TIM3->CCR2 = valeur_min_moteur;

	//HAL_Delay(3000);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {

		__HAL_UART_CLEAR_OREFLAG(&huart2);

		// Re-initialization of buffer
		r_buffer[0] = 0;

		//---------changing states-------

		switch (etat) {

		// First step: Press ENTER to start the experiment
		case entrance:

			do {
				HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 1, 10);
			} while (r_buffer[0] != '\r');

			etat = init_uc;

			break;

		// State init: initialization of micro-controller
		case init_uc:

			printf("State: Init uc\n\r");

			// -> démarrer le compteur
			//HAL_Delay(1000);
			//timer_val = __HAL_TIM_GET_COUNTER(&htim15);
			//HAL_Delay(6);
			//timer_val = __HAL_TIM_GET_COUNTER(&htim15) - timer_val ;
			//printf("Time consumed : %d *10^-6 secondes\r\n", timer_val);

			//HAL_Delay(1000);
			//timer_val2 = __HAL_TIM_GET_COUNTER(&htim2);
			//HAL_Delay(10);
			//timer_val2 = __HAL_TIM_GET_COUNTER(&htim2) - timer_val2 ;
			//HAL_TIM_Base_Stop(&htim2);
			//printf("Time consumed : %" PRIu32 " *10^-7 secondes\r\n", timer_val2);

			printf(msg_motor_ready);
			printf(msg_info_mode);

			printf("\n\r");
			do {
				if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 1, 10) == HAL_OK) {
					HAL_UART_Transmit(&huart2, (uint8_t*) r_buffer, 1, 10);
			    } else {
					__HAL_UART_CLEAR_OREFLAG(&huart2);
				}
			} while ((r_buffer[0] != ' ') && (r_buffer[0] != '0') && (r_buffer[0] != 'i'));
			printf("\n\n\r");

			switch (r_buffer[0]) {
			case ' ':
				etat = motor_ready;
				break;
			case '0':
				etat = motor_ready;
				break;
			case 'i':
				etat = info_mode;
				previous_etat = init_uc;
				break;
			default:
				break;
			}

			break;

		// State 'i': board of informations
		case info_mode:

			printf("State: Info mode\n\n\r");

			printf("Firmware version %.2f \n\r", firmware_version);
			printf("Baud rate %lu \n\n\r", huart2.Init.BaudRate);

			printf("Angle (auto mode) = %d deg\n\n\r", angle_term);
			printf("kp = %.4f\n\r", kp);
			printf("ki = %.4f\n\r", ki);
			printf("kd = %.4f\n\n\r", kd);

			printf("List of States:\n\r");
			printf(">>Init UC\n\r");
			printf(">>Info Mode\n\r");
			printf(">>Motor Ready\n\r");
			printf(">>Manual Mode Pot\n\r");
			printf(">>Manual Mode Term\n\r");
			printf(">>Auto Mode\n\n\r");

			//go to previous state
			etat = previous_etat;
			break;

		// State '0' | ' ': motor is ready
		case motor_ready:

			printf("State: Motor ready\n\n\r");

			printf(msg_info_mode);
			printf("> Press [ 1 ] for Manual mode pot\n\r");
			printf("> Press [ 2 ] for Manual mode term\n\r");
			printf("> Press [ 3 ] for Auto mode\n\r");


			// Init Motor
			load_pwm(htim3, valeur_min_moteur);
			HAL_Delay(100);

			printf("\n\r");
			do {
				if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 1, 10) == HAL_OK) {
					HAL_UART_Transmit(&huart2, (uint8_t*) r_buffer, 1, 10);
				} else {
					__HAL_UART_CLEAR_OREFLAG(&huart2);
				}
			} while ((r_buffer[0] != 'i') && (r_buffer[0] != '1')
					&& (r_buffer[0] != '2') && (r_buffer[0] != '3'));
			printf("\n\n\r");

			switch (r_buffer[0]) {
			case 'i':
				etat = info_mode;
				previous_etat = motor_ready;
				break;
			case '1':
				etat = manual_mode_pot;
				break;
			case '2':
				etat = manual_mode_term;
				break;
			case '3':
				etat = init_gyro;
				break;
			default:
				break;
			}

			break;

		// State '1': manual mode pot
		case manual_mode_pot:

			printf("State: Manual mode pot \n\n\r");

			// Start Init Pot--------------------
			printf("Init pot\n\r");

			HAL_ADC_Start(&hadc1);
			okay = 1;
			printf("> Put the potentiometer at zero\n\r");
			HAL_Delay(100);

			while (okay == 1) {
				HAL_ADC_Start(&hadc1);
				if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
					valeur_can = HAL_ADC_GetValue(&hadc1);
					printf("ADC value : %d \n\r", valeur_can);
				} else {
					printf("Conversion NOK\n\r");
				}

				if (valeur_can <= 20) {
					okay = 0;
				}

				HAL_Delay(1000);
			}

			printf("Success \n\n\r");
			// End Init Pot----------------------

			printf(msg_info_mode);
			printf(msg_motor_ready);

			//recuperation de la pwm
			printf("\n\r");
			do {
				if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 1, 10) == HAL_OK) {
					HAL_UART_Transmit(&huart2, (uint8_t*) r_buffer, 1, 10);
				} else {
					__HAL_UART_CLEAR_OREFLAG(&huart2);
				}
				valeur_can = load_adc(hadc1, 5);
				mapped_value = mapping_adc_value(valeur_can);
				HAL_Delay(100);
				load_pwm(htim3, mapped_value);

			} while (r_buffer[0] != '0' && (r_buffer[0] != ' ') && (r_buffer[0] != 'i'));
			printf("\n\n\r");

			switch (r_buffer[0]) {
			case 'i':
				etat = info_mode;
				previous_etat = manual_mode_pot;
				break;
			case '0':
				landing_value = mapped_value;
				etat = landing;
				break;
			case ' ':
				landing_value = mapped_value;
				etat = landing;
				break;
			default:
				break;
			}

			break;

		// State '2': manual mode term
		case manual_mode_term:

			printf("State: Manual mode term\n\n\r");

			// for now let's say 10 as value max -> 15% = dangerous
			// edit: it can now go up to 10% but only by incrementing by 1% with '+'
			printf("> Enter value between 1 and 10 (power percentage) then press [ ENTER ]\n\r");
			printf("> Press [ + ] or [ - ]\n\n\r");
			printf(msg_info_mode);
			printf(msg_motor_ready);

			//recovery of the pwm
			printf("\n\r");
			do {
				if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 1, 1) == HAL_OK) {
					r_buffer_string[cpt_char] = r_buffer[0];
					HAL_UART_Transmit(&huart2, (uint8_t*) r_buffer, 1, 1);
					if (cpt_char < max_cpt_char) {
						cpt_char++;
					}
				} else {
					__HAL_UART_CLEAR_OREFLAG(&huart2);
				}
			} while ((r_buffer[0] != '\r') && (r_buffer[0] != '+')
					&& (r_buffer[0] != '-') && (r_buffer[0] != 'i') && (r_buffer_string[0] != '0') && (r_buffer_string[2] != '0') && (r_buffer[0] != ' '));
			printf("\n\n\r");

			int value0 = r_buffer_string[0] - '0';
			int value1 = r_buffer_string[1] - '0';
			int value2 = r_buffer_string[2] - '0';

			if (cpt_char == 2) {
				if ((value0 > 9)||(value0 < 0)){
					printf(msg_error_char_nb);
				} else{
					int prov_gaz_term_percent = value0;
					if (prov_gaz_term_percent > 10){
						printf(msg_error_value_sup);
					} else{
						gaz_term_percent = prov_gaz_term_percent;
						printf("Gaz Term : %d\n\r", gaz_term_percent);
					}
				}
			}
			if (cpt_char == 3) {
				if ((value0 > 9)||(value0 < 0)||(value1 > 9)||(value1 < 0)){
					printf(msg_error_char_nb);
				} else{
					 int prov_gaz_term_percent = value0 * 10 + value1;
					if (prov_gaz_term_percent > 10){
						printf(msg_error_value_sup);
					} else{
						gaz_term_percent = prov_gaz_term_percent;
						printf("Gaz Term : %d\n\r", gaz_term_percent);
					}
				}
			}
			if (cpt_char == 4) {
				if ((value0 > 9)||(value0 < 0)||(value1 > 9)||(value1 < 0)||(value2 > 9)||(value2 < 0)){
					printf(msg_error_char_nb);
				} else{
					int prov_gaz_term_percent = value0 * 100 + value1 * 10 + value2;
					if (prov_gaz_term_percent > 10){
						printf(msg_error_value_sup);
					} else{
						gaz_term_percent = prov_gaz_term_percent;
						printf("Gaz Term : %d\n\r", gaz_term_percent);
					}
				}
			}

			if (r_buffer[0] == '+') {
				gaz_term_percent++;
				printf("Gaz Term : %d\n\r", gaz_term_percent);
			}
			if (r_buffer[0] == '-') {
				gaz_term_percent--;
				printf("Gaz Term : %d\n\r", gaz_term_percent);
			}

			mapped_value = mapping_adc_value_percent(gaz_term_percent);

			printf("Mapping adc value percent : %d\n\n\r", mapped_value);
			load_pwm(htim3, mapped_value);
			HAL_Delay(100);

			if (gaz_term_percent == 0) {
				landing_value = mapped_value;
				mapped_value = valeur_min_moteur;
				etat = landing;
			} else {
				etat = manual_mode_term;
			}

			if (r_buffer[0] == 'i'){
				etat = info_mode;
				previous_etat = manual_mode_term;
			}
			if (r_buffer[0] == ' ') {
				landing_value = mapped_value;
				mapped_value = valeur_min_moteur;
				etat = landing;
				gaz_term_percent = 0;
			}
			if (r_buffer_string[0] == '0') {
				landing_value = mapped_value;
				mapped_value = valeur_min_moteur;
				etat = landing;
				gaz_term_percent = 0;
			}

			r_buffer_string[0] = 0;
			r_buffer_string[1] = 0;
			r_buffer_string[2] = 0;
			value0 = 0;
			value1 = 0;
			value2 = 0;
			cpt_char = 0;

			break;

		// State '3': auto mode

		case init_gyro:

			printf("State: Auto mode\n\n\r");

			printf("Waiting for Gyro MPU6050...\n\r");
			while (MPU6050_Init(&hi2c1) == 1)
				;
			printf("Gyro MPU6050 OK!\n\n\r");

			if (MPU6050_Init(&hi2c1) == 0) {
				printf("Gyro MPU6050 initialized\n\n\r");
				etat = instruct_angle;
			} else {
				printf("Gyro MPU6050 is not working\n\n\r");
				etat = motor_ready;
				break;
			}
			break;

		case instruct_angle:

			printf(msg_info_mode);
			printf(msg_motor_ready);
			// Ask for instructions for the angle of the arm
			printf("> Enter value between 1 and 90 degrees (angle of the arm) then press [ ENTER ]\n\r");

			printf("\n\r");
			do {
				if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 1, 1) == HAL_OK) {
					r_buffer_string_prime[cpt_char_prime] = r_buffer[0];
					HAL_UART_Transmit(&huart2, (uint8_t*) r_buffer, 1, 1);
					if (cpt_char_prime < max_cpt_char_prime) {
						cpt_char_prime++;
					}
				} else {
					__HAL_UART_CLEAR_OREFLAG(&huart2);
				}
			} while ((r_buffer[0] != '\r') && (r_buffer[0] != 'i') && (r_buffer_string_prime[0] != '0') && (r_buffer[0] != ' '));
			printf("\n\n\r");

			int value_prime0 = r_buffer_string_prime[0] - '0';
			int value_prime1 = r_buffer_string_prime[1] - '0';

			if (cpt_char_prime == 2) {
				if ((value_prime0 > 9)||(value_prime0 < 0)){
					printf(msg_error_char_nb);
					etat = instruct_angle;
				} else{
					int prov_angle_term = value_prime0;
					if (prov_angle_term > 90){
						printf(msg_error_value_sup_angle);
						etat = instruct_angle;
					} else{
						angle_term = prov_angle_term;
						printf("Angle : %d degree(s)\n\n\r", angle_term);
						etat = auto_mode;
					}
				}
			}
			if (cpt_char_prime == 3) {
				if ((value_prime0 > 9)||(value_prime0 < 0)||(value_prime1 > 9)||(value_prime1 < 0)){
					printf(msg_error_char_nb);
					etat = instruct_angle;
				} else{
					int prov_angle_term = value_prime0 * 10 + value_prime1;
					if (prov_angle_term > 90){
						printf(msg_error_value_sup_angle);
						etat = instruct_angle;
					} else{
						angle_term = prov_angle_term;
						printf("Angle : %d degree(s)\n\n\r", angle_term);
						etat = auto_mode;
					}
				}
			}

			if (r_buffer[0] == 'i') {
				etat = info_mode;
				previous_etat = instruct_angle;
			}
			if (r_buffer[0] == ' ') {
				landing_value = commande;
				commande = valeur_min_moteur;
				etat = landing;
				integre_erreur = 0;
				erreur = 0;
			}
			if (r_buffer_string_prime[0] == '0') {
				landing_value = commande;
				commande = valeur_min_moteur;
				etat = landing;
				integre_erreur = 0;
				erreur = 0;
			}

			r_buffer_string_prime[0] = 0;
			r_buffer_string_prime[1] = 0;
			value_prime0 = 0;
			value_prime1 = 0;
			cpt_char_prime = 0;
			break;

		case instruct_kp:

			printf(msg_info_mode);
			printf(msg_motor_ready);

			// Ask for instructions for the coefficient kp
			printf("> kp coefficient looks like [0.****].\n\rPlease enter all the values at the [*] then press [ ENTER ]\n\r");

			printf("\n\r");
			do {
				if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 1, 1)
						== HAL_OK) {
					r_buffer_string_kp[cpt_char_kp] = r_buffer[0];
					HAL_UART_Transmit(&huart2, (uint8_t*) r_buffer, 1, 1);
					if (cpt_char_kp < max_cpt_char_kp) {
						cpt_char_kp++;
					}
				} else {
					__HAL_UART_CLEAR_OREFLAG(&huart2);
				}
			} while ((r_buffer[0] != '\r') && (r_buffer[0] != 'i') && (r_buffer[0] != ' '));
			printf("\n\n\r");
			//----------------------------------------------------------------------------------------------------------------------------
			int value_kp0 = r_buffer_string_kp[0] - '0';
			printf("Value kp0 = %d\n\r", value_kp0);
			int value_kp1 = r_buffer_string_kp[1] - '0';
			printf("Value kp1 = %d\n\r", value_kp1);
			int value_kp2 = r_buffer_string_kp[2] - '0';
			printf("Value kp2 = %d\n\r", value_kp2);
			int value_kp3 = r_buffer_string_kp[3] - '0';
			printf("Value kp3 = %d\n\r", value_kp3);

			if (cpt_char_kp == 2) {
				if ((value_kp0 > 9)||(value_kp0 < 0)){
					printf(msg_error_char_nb);
					etat = instruct_kp;
				} else{
					kp =value_kp0 /10.;
					printf("kp = %.4f \n\r", kp);
					etat = auto_mode;
				}
			}
			if (cpt_char_kp == 3) {
				if ((value_kp0 > 9)||(value_kp0 < 0)||(value_kp1 > 9)||(value_kp1 < 0)){
					printf(msg_error_char_nb);
					etat = instruct_kp;
				} else{
					kp = value_kp0 /10. + value_kp1 /100.;
					printf("kp = %.4f \n\r", kp);
					etat = auto_mode;
				}
			}
			if (cpt_char_kp == 4) {
				if ((value_kp0 > 9)||(value_kp0 < 0)||(value_kp1 > 9)||(value_kp1 < 0)||(value_kp2 > 9)||(value_kp2 < 0)){
					printf(msg_error_char_nb);
					etat = instruct_kp;
				} else{
					kp = value_kp0 /10. + value_kp1 /100. + value_kp2 /1000.;
					printf("kp = %.4f \n\r", kp);
					etat = auto_mode;
				}
			}
			if (cpt_char_kp == 5) {
				if ((value_kp0 > 9)||(value_kp0 < 0)||(value_kp1 > 9)||(value_kp1 < 0)||(value_kp2 > 9)||(value_kp2 < 0)||(value_kp3 > 9)||(value_kp3 < 0)){
					printf(msg_error_char_nb);
					etat = instruct_kp;
				} else{
					kp = value_kp0 /10. + value_kp1 /100. + value_kp2 /1000. + value_kp3 /10000.;
					printf("kp = %.4f \n\r", kp);
					etat = auto_mode;
				}
			}

			if (r_buffer[0] == 'i') {
				etat = info_mode;
				previous_etat = instruct_kp;
			}
			if (r_buffer[0] == ' ') {
				landing_value = commande;
				commande = valeur_min_moteur;
				etat = landing;
				integre_erreur = 0;
				erreur = 0;
			}

			r_buffer_string_kp[0] = 0;
			r_buffer_string_kp[1] = 0;
			r_buffer_string_kp[2] = 0;
			r_buffer_string_kp[3] = 0;
			value_kp0 = 0;
			value_kp1 = 0;
			value_kp2 = 0;
			value_kp3 = 0;
			cpt_char_kp = 0;

			//----------------------------------------------------------------------------------------------------------------------------

			// --------------------------------- TEST ------------------------------------------------------------------------------------
			//int value_kp = 0;
			//int kp_prov = 0;
			//int test_err_kp = 0;
			//int counter_comma_pos = 0;// -> condition par rapport à position 0 (car impossible d'avoir la virgule en 1ere entrée)
			//int counter_comma_nb = 0;// -> nombre de virgules : si plus d'une -> erreur

			//for (int i = 0; i = cpt_char_kp -2; ++i){
				//value_kp = r_buffer_string_kp[i];
				//if (value_kp == 44 || value_kp == 46){
					//counter_comma_pos = i;
					//counter_comma_nb++;
				//} else{
					//value_kp = r_buffer_string_kp[0] - '0';
					//if (value_kp<0 || value_kp>9){
						//test_err_kp++;
						//printf(msg_error_char_nb);
					//} else{
						// kp_prov = kp_prov + ; A POURSUIVRE
					//}
				//}

			//}

			//int value_kp0 = r_buffer_string_kp[0] - '0';
			//int value_kp1 = r_buffer_string_kp[1] - '0';




			// à continuer ----------------------------------------------------------------------------------------------------------------

			break;

		case instruct_ki:

		printf(msg_info_mode);
		printf(msg_motor_ready);

		// Ask for instructions for the coefficient ki
		printf("> ki coefficient looks like [0.****].\n\rPlease enter all the values at the [*] then press [ ENTER ]\n\r");

		printf("\n\r");
		do {
			if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 1, 1)
					== HAL_OK) {
				r_buffer_string_ki[cpt_char_ki] = r_buffer[0];
				HAL_UART_Transmit(&huart2, (uint8_t*) r_buffer, 1, 1);
				if (cpt_char_ki < max_cpt_char_ki) {
					cpt_char_ki++;
				}
			} else {
				__HAL_UART_CLEAR_OREFLAG(&huart2);
			}
		} while ((r_buffer[0] != '\r') && (r_buffer[0] != 'i') && (r_buffer[0] != ' '));
		printf("\n\n\r");
		//----------------------------------------------------------------------------------------------------------------------------
		int value_ki0 = r_buffer_string_ki[0] - '0';
		printf("Value ki0 = %d\n\r", value_ki0);
		int value_ki1 = r_buffer_string_ki[1] - '0';
		printf("Value ki1 = %d\n\r", value_ki1);
		int value_ki2 = r_buffer_string_ki[2] - '0';
		printf("Value ki2 = %d\n\r", value_ki2);
		int value_ki3 = r_buffer_string_ki[3] - '0';
		printf("Value ki3 = %d\n\r", value_ki3);

		if (cpt_char_ki == 2) {
			if ((value_ki0 > 9)||(value_ki0 < 0)) {
				printf(msg_error_char_nb);
				etat = instruct_ki;
			} else{
				ki =value_ki0 /10.;
				printf("ki = %.4f \n\r", ki);
				etat = auto_mode;
			}
		}
		if (cpt_char_ki == 3) {
			if ((value_ki0 > 9)||(value_ki0 < 0)||(value_ki1 > 9)||(value_ki1 < 0)){
				printf(msg_error_char_nb);
				etat = instruct_ki;
			} else{
				ki = value_ki0 /10. + value_ki1 /100.;
				printf("ki = %.4f \n\r", ki);
				etat = auto_mode;
			}
		}
		if (cpt_char_ki == 4) {
			if ((value_ki0 > 9)||(value_ki0 < 0)||(value_ki1 > 9)||(value_ki1 < 0)||(value_ki2 > 9)||(value_ki2 < 0)){
				printf(msg_error_char_nb);
				etat = instruct_ki;
			} else{
				ki = value_ki0 /10. + value_ki1 /100. + value_ki2 /1000.;
				printf("ki = %.4f \n\r", ki);
				etat = auto_mode;
			}
		}
		if (cpt_char_ki == 5) {
			if ((value_ki0 > 9)||(value_ki0 < 0)||(value_ki1 > 9)||(value_ki1 < 0)||(value_ki2 > 9)||(value_ki2 < 0)||(value_ki3 > 9)||(value_ki3 < 0)){
					printf(msg_error_char_nb);
				etat = instruct_ki;
			} else{
				ki = value_ki0 /10. + value_ki1 /100. + value_ki2 /1000. + value_ki3 /10000.;
				printf("ki = %.4f \n\r", ki);
				etat = auto_mode;
			}
		}

		if (r_buffer[0] == 'i') {
			etat = info_mode;
			previous_etat = instruct_ki;
		}
		if (r_buffer[0] == ' ') {
			landing_value = commande;
			commande = valeur_min_moteur;
			etat = landing;
			integre_erreur = 0;
			erreur = 0;
		}

		r_buffer_string_ki[0] = 0;
		r_buffer_string_ki[1] = 0;
		r_buffer_string_ki[2] = 0;
		r_buffer_string_ki[3] = 0;
		value_ki0 = 0;
		value_ki1 = 0;
		value_ki2 = 0;
		value_ki3 = 0;
		cpt_char_ki = 0;

		//----------------------------------------------------------------------------------------------------------------------------

			break;

		case instruct_kd:

		printf(msg_info_mode);
		printf(msg_motor_ready);

		// Ask for instructions for the coefficient kd
		printf("> kd coefficient looks like [0.****].\n\rPlease enter all the values at the [*] then press [ ENTER ]\n\r");

		printf("\n\r");
		do {
			if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 1, 1)
					== HAL_OK) {
				r_buffer_string_kd[cpt_char_kd] = r_buffer[0];
				HAL_UART_Transmit(&huart2, (uint8_t*) r_buffer, 1, 1);
				if (cpt_char_kd < max_cpt_char_kd) {
					cpt_char_kd++;
				}
			} else {
				__HAL_UART_CLEAR_OREFLAG(&huart2);
			}
		} while ((r_buffer[0] != '\r') && (r_buffer[0] != 'i') && (r_buffer[0] != ' '));
		printf("\n\n\r");
		//----------------------------------------------------------------------------------------------------------------------------
		int value_kd0 = r_buffer_string_kd[0] - '0';
		printf("Value kd0 = %d\n\r", value_kd0);
		int value_kd1 = r_buffer_string_kd[1] - '0';
		printf("Value kd1 = %d\n\r", value_kd1);
		int value_kd2 = r_buffer_string_kd[2] - '0';
		printf("Value kd2 = %d\n\r", value_kd2);
		int value_kd3 = r_buffer_string_kd[3] - '0';
		printf("Value kd3 = %d\n\r", value_kd3);

		if (cpt_char_kd == 2) {
			if ((value_kd0 > 9)||(value_kd0 < 0)){
				printf(msg_error_char_nb);
				etat = instruct_kd;
			} else{
				kd =value_kd0 /10.;
				printf("kd = %.4f \n\r", kd);
				etat = auto_mode;
			}
		}
		if (cpt_char_kd == 3) {
			if ((value_kd0 > 9)||(value_kd0 < 0)||(value_kd1 > 9)||(value_kd1 < 0)){
				printf(msg_error_char_nb);
				etat = instruct_kd;
			} else{
				kd = value_kd0 /10. + value_kd1 /100.;
				printf("kd = %.4f \n\r", kd);
				etat = auto_mode;
			}
		}
		if (cpt_char_kd == 4) {
			if ((value_kd0 > 9)||(value_kd0 < 0)||(value_kd1 > 9)||(value_kd1 < 0)||(value_kd2 > 9)||(value_kd2 < 0)){
				printf(msg_error_char_nb);
				etat = instruct_kd;
			} else{
				kd = value_kd0 /10. + value_kd1 /100. + value_kd2 /1000.;
				printf("kd = %.4f \n\r", kd);
				etat = auto_mode;
			}
		}
		if (cpt_char_kd == 5) {
			if ((value_kd0 > 9)||(value_kd0 < 0)||(value_kd1 > 9)||(value_kd1 < 0)||(value_kd2 > 9)||(value_kd2 < 0)||(value_kd3 > 9)||(value_kd3 < 0)){
				printf(msg_error_char_nb);
				etat = instruct_kd;
			} else{
				kd = value_kd0 /10. + value_kd1 /100. + value_kd2 /1000. + value_kd3 /10000.;
				printf("kd = %.4f \n\r", kd);
				etat = auto_mode;
			}
		}

		if (r_buffer[0] == 'i') {
			etat = info_mode;
			previous_etat = instruct_kd;
		}
		if (r_buffer[0] == ' ') {
			landing_value = commande;
			commande = valeur_min_moteur;
			etat = landing;
			integre_erreur = 0;
			erreur = 0;
		}

		r_buffer_string_kd[0] = 0;
		r_buffer_string_kd[1] = 0;
		r_buffer_string_kd[2] = 0;
		r_buffer_string_kd[3] = 0;
		value_kd0 = 0;
		value_kd1 = 0;
		value_kd2 = 0;
		value_kd3 = 0;
		cpt_char_kd = 0;

		//----------------------------------------------------------------------------------------------------------------------------

			break;

		case auto_mode:

			printf(msg_info_mode);
			printf(msg_motor_ready);
			printf("> Please enter [x] if you want to modify kp value\n\rPlease enter [y] if you want to modify ki value\n\rPlease enter [z] if you want to modify kd value\n\r");
			printf("> Please enter [w] if you want to modify the angle value\n\r");
			printf("> If you want to come back to default set of PID coefficients then press [d]\n\r");

			consigne = angle_term;
			printf("\n\r");


			// -----------------------------------------
			HAL_Delay(1000);
			timer_val2 = __HAL_TIM_GET_COUNTER(&htim2);
			do {
				if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 1, 10) == HAL_OK) {
					HAL_UART_Transmit(&huart2, (uint8_t*) r_buffer, 1, 10);
					printf("\n\n\r");
				} else {
					__HAL_UART_CLEAR_OREFLAG(&huart2);
				}

				MPU6050_Read_All(&hi2c1, &mpu);
				//MPU6050_Read_Accel(&hi2c1, &mpu);
				//MPU6050_Read_Gyro(&hi2c1, &mpu);
				//Kalman_getAngle(&KalmanX, roll, DataStruct->Gx, dt);

				position_angulaire = mpu.KalmanAngleX + 90;

				// Asservissement

				_erreur = erreur;
				erreur = consigne - position_angulaire;
				integre_erreur += erreur;
				derive_erreur = erreur - _erreur;
				commande = kp * (erreur) + ki * (integre_erreur) + kd * (derive_erreur);

				if (commande > valeur_max_moteur) {
					commande = valeur_max_moteur;
				}
				if (commande < valeur_min_moteur) {
					commande = valeur_min_moteur;
				}

				load_pwm(htim3, commande);
			} while (r_buffer[0] != '0' && (r_buffer[0] != ' ') && (r_buffer[0] != 'i') && (r_buffer[0] != 'd') && (r_buffer[0] != 'w') && (r_buffer[0] != 'x') && (r_buffer[0] != 'y') && (r_buffer[0] != 'z'));
			timer_val2 = __HAL_TIM_GET_COUNTER(&htim2) - timer_val2 ;
			//HAL_TIM_Base_Stop(&htim2);
			printf("Time consumed : %" PRIu32 " *10^-7 seconds\r\n", timer_val2);
			// -----------------------------------------


			if (r_buffer[0] == '0'){
				landing_value = commande;
				commande = valeur_min_moteur;
				etat = landing;
				integre_erreur = 0;
				erreur = 0;
			}

			if (r_buffer[0] == ' '){
				landing_value = commande;
				commande = valeur_min_moteur;
				etat = landing;
				integre_erreur = 0;
				erreur = 0;
			}
			if (r_buffer[0] == 'i'){
				etat = info_mode;
				previous_etat = auto_mode;
			}
			if (r_buffer[0] == 'd'){
				kp = 0.001;
				ki = 0.018;
				kd = 0.1;
				etat = auto_mode;
			}
			if (r_buffer[0] == 'w'){
				etat = instruct_angle;
			}
			if (r_buffer[0] == 'x'){
				etat = instruct_kp;
			}
			if (r_buffer[0] == 'y'){
				etat = instruct_ki;
			}
			if (r_buffer[0] == 'z'){
				etat = instruct_kd;
			}

			break;

		case landing:

			do{
				landing_value--;
				load_pwm(htim3, landing_value);
				HAL_Delay(25);
			} while(landing_value>valeur_min_moteur);

			etat = motor_ready;

			break;

		default:
			break;

		}
		//---------changing states----END---

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
