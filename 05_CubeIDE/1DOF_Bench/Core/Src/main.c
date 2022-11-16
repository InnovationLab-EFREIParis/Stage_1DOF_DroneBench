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
int main(void) {
	/* USER CODE BEGIN 1 */

	enum states etat, previous_etat;
	etat = init_uc;
	char r_buffer[2];
	int okay;
	int valeur_can;
	int mapped_value;
	double position_angulaire;

	//double consigne = 45;
	double commande = valeur_min_moteur;

	// Coeff OK
	/*double kp=0.0001;
	 double ki=0.02;
	 double kd=0.001;*/
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

	int cpt_char_kp = 0;
	int max_cpt_char_kp = 7;
	char r_buffer_string_kp[max_cpt_char_kp];

	int cpt_char_ki = 0;
	int max_cpt_char_ki = 7;
	char r_buffer_string_ki[max_cpt_char_ki];

	int cpt_char_kd = 0;
	int max_cpt_char_kd = 7;
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

		// State init: initialization of micro-controller
		case init_uc:

			//managing outputs
			printf("State: Init uc\n\r");

			printf(msg_motor_ready);
			printf(msg_info_mode);

			//managing inputs (transitions)
			do {
				HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 2, 10);
			} while ((r_buffer[0] != ' ') && (r_buffer[0] != '0') && (r_buffer[0] != 'i'));

			switch (r_buffer[0]) {
			case ' ':
				etat = motor_ready;
				break;
			case '0':
				etat = motor_ready;
				break;
			case 'i':
				etat = info_mode;
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
			printf("kp = %.4f\n\r",kp);
			printf("ki = %.4f\n\r",ki);
			printf("kd = %.4f\n\n\r",kd);


			printf("List of States:\n\r");
			printf(">>Init UC\n\r");
			printf(">>Info Mode\n\r");
			printf(">>Motor Ready\n\r");
			printf(">>Manual Mode Pot\n\r");
			printf(">>Manual Mode Term\n\r");
			printf(">>Auto Mode\n\n\r");

			//go to init uc
			etat = previous_etat;
			break;

		// State '0' | ' ': motor is ready
		case motor_ready:

			printf("State: Motor ready\n\n\r");

			printf(msg_info_mode);
			printf("> Press [ 1 ] for Manual mode pot\n\r");
			printf("> Press [ 2 ] for Manual mode term\n\r");
			printf("> Press [ 3 ] for Auto mode\n\n\r");


			// Init Motor
			load_pwm(htim3, valeur_min_moteur);
			HAL_Delay(100);

			do {
				if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 2, 10)
						== HAL_OK) {
					HAL_Delay(50);
				}
			} while ((r_buffer[0] != 'i') && (r_buffer[0] != '1')
					&& (r_buffer[0] != '2') && (r_buffer[0] != '3'));

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
					printf("ADC value :\t %d \n\r", valeur_can);
				} else {
					printf("Conversion NOK\n\r");
				}

				if (valeur_can <= 20) {
					okay = 0;
				}

				// home test condition -------
				//okay = 0;
				// end home test condition ---

				HAL_Delay(1000);
			}

			printf("Success \n\n\r");
			// End Init Pot----------------------

			printf(msg_info_mode);
			printf(msg_motor_ready);

			//recuperation de la pwm
			do {
				if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 2, 1)
						== HAL_OK)
					HAL_Delay(10);

				valeur_can = load_adc(hadc1, 5);
				mapped_value = mapping_adc_value(valeur_can);
				HAL_Delay(100);
				load_pwm(htim3, mapped_value);

			} while (r_buffer[0] != '0' && (r_buffer[0] != ' ') && (r_buffer[0] != 'i'));

			switch (r_buffer[0]) {
			case 'i':
				etat = info_mode;
				previous_etat = manual_mode_pot;
				break;
			case '0':
				etat = motor_ready;
				break;
			case ' ':
				etat = motor_ready;
				break;
			default:
				break;
			}

			break;

		// State '2': manual mode term
		case manual_mode_term:

			printf("State: Manual mode term\n\n\r");

			// for now let's say 10 as value max -> 15% = dangerous
			printf("> Enter value between 1 and 10 (power percentage) then press [ ENTER ]\n\r");
			printf("> Press [ + ] or [ - ]\n\n\r");
			printf(msg_info_mode);
			printf(msg_motor_ready);

			//recovery of the pwm
			do {
				if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 1, 1)
						== HAL_OK) {
					r_buffer_string[cpt_char] = r_buffer[0];
					if (cpt_char < max_cpt_char) {
						cpt_char++;
					}
				} else {
					__HAL_UART_CLEAR_OREFLAG(&huart2);
				}
			} while ((r_buffer[0] != '\r') && (r_buffer[0] != '+')
					&& (r_buffer[0] != '-') && (r_buffer[0] != 'i') && (r_buffer_string[0] != '0') && (r_buffer_string[2] != '0') && (r_buffer[0] != ' '));

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
					}
				}
			}

			if (r_buffer[0] == '+') {
				gaz_term_percent++;
			}
			if (r_buffer[0] == '-') {
				gaz_term_percent--;
			}
			if (r_buffer[0] == 'i') {
			}
			if (r_buffer[0] == ' ') {
				gaz_term_percent = 0;
			}
			if (r_buffer_string[0] == '0') {
				gaz_term_percent = 0;
			}

			printf("Gaz Term %d \n\r", gaz_term_percent);

			mapped_value = mapping_adc_value_percent(gaz_term_percent);

			printf("Mapping adc value percent %d\n\n\r", mapped_value);
			load_pwm(htim3, mapped_value);
			HAL_Delay(100);

			r_buffer_string[0] = 0;
			r_buffer_string[1] = 0;
			r_buffer_string[2] = 0;
			value0 = 0;
			value1 = 0;
			value2 = 0;
			cpt_char = 0;

			if (gaz_term_percent == 0) {
				etat = motor_ready;
			} else {
				etat = manual_mode_term;
			}

			if (r_buffer[0] == 'i'){
				etat = info_mode;
				previous_etat = manual_mode_term;
			}
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
			printf("> Enter value between 1 and 90 degrees (angle of the arm) then press [ ENTER ]\n\n\r");

			do {
				if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 1, 1)
						== HAL_OK) {
					r_buffer_string_prime[cpt_char_prime] = r_buffer[0];
					if (cpt_char_prime < max_cpt_char_prime) {
						cpt_char_prime++;
					}
				} else {
					__HAL_UART_CLEAR_OREFLAG(&huart2);
				}
			} while ((r_buffer[0] != '\r') && (r_buffer[0] != 'i') && (r_buffer_string_prime[0] != '0') && (r_buffer[0] != ' '));

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
						//etat = instruct_kp;
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
						//etat = instruct_kp;
					}
				}
			}

			if (r_buffer[0] == 'i') {
				etat = info_mode;
				previous_etat = instruct_angle;
			}
			if (r_buffer[0] == ' ') {
				etat = motor_ready;
			}
			if (r_buffer_string_prime[0] == '0') {
				etat = motor_ready;
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
			printf("> Enter value for the coefficient kp then press [ ENTER ]\n\n\r");

			do {
				if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 1, 1)
						== HAL_OK) {
					r_buffer_string_kp[cpt_char_kp] = r_buffer[0];
					if (cpt_char_kp < max_cpt_char_kp) {
						cpt_char_kp++;
					}
				} else {
					__HAL_UART_CLEAR_OREFLAG(&huart2);
				}
			} while ((r_buffer[0] != '\r') && (r_buffer[0] != 'i') && (r_buffer[0] != ' '));

			int value_kp = 0;
			int kp_prov = 0;
			int test_err_kp = 0;
			int counter_comma_pos = 0;// -> condition par rapport à position 0 (car impossible d'avoir la virgule en 1ere entrée)
			int counter_comma_nb = 0;// -> nombre de virgules : si plus d'une -> erreur

			for (int i = 0; i = cpt_char_kp -2; ++i){
				value_kp = r_buffer_string_kp[i];
				if (value_kp == 44 || value_kp == 46){
					counter_comma_pos = i;
					counter_comma_nb++;
				} else{
					value_kp = r_buffer_string_kp[0] - '0';
					if (value_kp<0 || value_kp>9){
						test_err_kp++;
						printf(msg_error_char_nb);
					} else{
						// kp_prov = kp_prov + ; A POURSUIVRE
					}
				}

			}

			int value_kp0 = r_buffer_string_kp[0] - '0';
			int value_kp1 = r_buffer_string_kp[1] - '0';




			// à continuer

			break;

		case instruct_ki:

		printf(msg_info_mode);
		printf(msg_motor_ready);

		// Ask for instructions for the coefficient ki
		printf("> Enter value for the coefficient ki then press [ ENTER ]\n\n\r");

		do {
			if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 1, 1)
					== HAL_OK) {
				r_buffer_string_ki[cpt_char_ki] = r_buffer[0];
				if (cpt_char_ki < max_cpt_char_ki) {
					cpt_char_ki++;
				}
			} else {
				__HAL_UART_CLEAR_OREFLAG(&huart2);
			}
		} while ((r_buffer[0] != '\r') && (r_buffer[0] != 'i') && (r_buffer[0] != ' '));

		int value_ki0 = r_buffer_string_ki[0] - '0';
		int value_ki1 = r_buffer_string_ki[1] - '0';

		// à continuer

			break;

		case instruct_kd:

		printf(msg_info_mode);
		printf(msg_motor_ready);

		// Ask for instructions for the coefficient kd
		printf("> Enter value for the coefficient kd then press [ ENTER ]\n\n\r");

		do {
			if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 1, 1)
					== HAL_OK) {
				r_buffer_string_kd[cpt_char_kd] = r_buffer[0];
				if (cpt_char_kd < max_cpt_char_kd) {
					cpt_char_kd++;
				}
			} else {
				__HAL_UART_CLEAR_OREFLAG(&huart2);
			}
		} while ((r_buffer[0] != '\r') && (r_buffer[0] != 'i') && (r_buffer[0] != ' '));

		int value_kd0 = r_buffer_string_kd[0] - '0';
		int value_kd1 = r_buffer_string_kd[1] - '0';

		// à continuer

			break;

		case auto_mode:

			printf(msg_info_mode);
			printf(msg_motor_ready);
			//consigne = angle_term;
			integre_erreur = 0;

			do {
				if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 2, 10)
						== HAL_OK) {
				}
				//pseudo asservisssement à 45deg
				MPU6050_Read_All(&hi2c1, &mpu);
				//MPU6050_Read_Accel(&hi2c1, &mpu);
				//MPU6050_Read_Gyro(&hi2c1, &mpu);
				//Kalman_getAngle(&KalmanX, roll, DataStruct->Gx, dt);

				//position_angulaire = 90 - mpu.KalmanAngleX;
				position_angulaire = mpu.KalmanAngleX + 90;
				//printf("Position %.2f commande %.2f \n\r", position_angulaire, commande);HAL_Delay(500);

				// Asservissement
				_erreur = erreur;
				//erreur = consigne - position_angulaire;
				erreur = angle_term - position_angulaire;

				integre_erreur += erreur;
				derive_erreur = erreur - _erreur;

				commande = kp * (erreur) + ki * (integre_erreur)
						+ kd * (derive_erreur);
				//printf("pos %.1f err %.1f com %.1f\n\r", position_angulaire, erreur, commande); HAL_Delay(500);
				if (commande > valeur_max_moteur) {
					commande = valeur_max_moteur;
				}
				if (commande < valeur_min_moteur) {
					commande = valeur_min_moteur;
				}

				load_pwm(htim3, commande);


			} while (r_buffer[0] != '0' && (r_buffer[0] != ' ') && (r_buffer[0] != 'i'));

			if (r_buffer[0] == '0'){
				etat = motor_ready;
			}

			if (r_buffer[0] == ' '){
				etat = motor_ready;
			}
			if (r_buffer[0] == 'i'){
				etat = info_mode;
				previous_etat = auto_mode;
			}

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
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1)
			!= HAL_OK) {
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
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
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
