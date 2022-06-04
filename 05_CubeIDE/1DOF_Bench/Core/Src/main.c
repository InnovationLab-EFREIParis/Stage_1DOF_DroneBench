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
 * 	scl=D8
 * 	sda = D9
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
int main(void)
{
  /* USER CODE BEGIN 1 */

	enum states etat;
	etat = init_uc;
	char r_buffer[2];
	int okay;
	int valeur_can;
	int mapped_value;
	// char gaz_data[4];
	//	int counter=4;
	double true_angle;
	//char buffer [size];

	int k = 1700;	//var used to increement speed in auto state
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
	//while (MPU6050_Init(&hi2c1) == 1)
	//	;
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
	//y_print(&huart2, " 0 to 6 to change state \r\n", 26);
	//printf("0 to 6 to change state \r\n");
	//HAL_Delay(3000);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
		__HAL_UART_CLEAR_OREFLAG(&huart2);

		//differents etats qu'on peut avoir

		//---------changement d'etat-------

		switch (etat) {

		case init_uc:
			//traitement des sorties
			/*if (HAL_UART_Transmit(&huart2, (uint8_t*) "UC Initialization \n\r", 22,
			 100) != HAL_OK)
			 Error_Handler();*/
			printf("State: Init uc\r\n");
			printf("> Press 2 for info mode\r\n");
			//printf("nucleo ready\r\n");
			//gyro init
			MPU6050_Read_All(&hi2c1, &mpu);
			HAL_Delay(1000);
			//traitement des entrées (transitions)
			do {

				if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 2, 10)
						== HAL_OK) {
					HAL_Delay(50);
					HAL_UART_Transmit(&huart2, (uint8_t*) r_buffer, 2, 10);
					HAL_Delay(50);
				}

			} while ((r_buffer[0] != '1') && (r_buffer[0] != '2')); //|| (r_buffer[0] != '2')
			if (r_buffer[0] == '2')
				etat = info_mode;
			else
				etat = init_motor;
			r_buffer[0] = ' ';
			break;

		case info_mode:
			//if (HAL_UART_Transmit(&huart2, (uint8_t*) "Info mode\n\r", 12, 100)
			//	!= HAL_OK)
			//Error_Handler();
			printf("State: Info mode\r\n");
			printf(">Firmware version %.2f \n\r", firmware_version);
			//HAL_Delay(3000);
			printf(">1 Init Motor \n\r");
			printf(">2 Info Mode \n\r");
			printf(">3 Init Pot \n\r");
			printf(">4 Auto Mode \n\r");
			printf(">5 Manual Mode \n\r");

			//sortie de la boucle
			/*do {
			 __HAL_UART_CLEAR_OREFLAG(&huart2);
			 if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 2, 10)
			 == HAL_OK) {
			 HAL_Delay(50);
			 HAL_UART_Transmit(&huart2, (uint8_t*) r_buffer, 2, 10);
			 HAL_Delay(50);
			 }

			 } while (r_buffer[0] != '0');*/

			etat = init_uc;
			// Reinitialisation du buffer
			r_buffer[0] = 0;
			//printf("%f");
			//le programme freeze dans l'etat info
			break;

		case init_motor:
			/*if (HAL_UART_Transmit(&huart2,
			 (uint8_t*) "Motor Initialization \r\n", 24, 100) != HAL_OK)
			 Error_Handler();*/
			printf("State: Motor Initialization \n\r");
			HAL_Delay(1000);
			//Chargement de la pwm
			//HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
			load_pwm(htim3, valeur_min_moteur);
			r_buffer[0] = ' ';
			etat = motor_ready;

			break;

		case motor_ready:
			/*	if (HAL_UART_Transmit(&huart2, (uint8_t*) "Motor ready \n\r", 15,
			 100) != HAL_OK)
			 Error_Handler();
			 HAL_Delay(3000);*/
			k = 1700;
			load_pwm(htim3, valeur_min_moteur);
			printf("State: Motor ready \n\r");
			HAL_Delay(1000);

			do {

				if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 2, 10)
						== HAL_OK) {
					HAL_Delay(50);
					HAL_UART_Transmit(&huart2, (uint8_t*) r_buffer, 2, 10);
					HAL_Delay(50);
				}

			} while ((r_buffer[0] != '3') && (r_buffer[0] != '4')); //|| (r_buffer[0] != '2')
			if (r_buffer[0] == '3')
				etat = init_pot;
			else
				etat = auto_mode;
			r_buffer[0] = ' ';

			break;

		case auto_mode:
			//if (HAL_UART_Transmit(&huart2, (uint8_t*) "Auto mode \n\r", 15, 100)
			//	!= HAL_OK)
			//Error_Handler();
			//HAL_Delay(3000);

			printf("State: Auto mode \n\r");

			do {

				if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 2, 10)
						== HAL_OK)
					HAL_Delay(10);
				//HAL_UART_Transmit(&huart2, (uint8_t*) r_buffer, 2, 10);
				//HAL_Delay(50);

			} while (r_buffer[0] != '6' && r_buffer[0] != 'g');

			//quand on envoie le caratère g on se retrouve dans la phase de gaz sur le clavier
			//si 6 on retourne au mode ready

			if (r_buffer[0] == '6') {
				etat = motor_ready;
				r_buffer[0] = ' ';
				break;

				//le but pour l'entrée des gaz sera de mettre une valeur, la traiter et retourner en mode auto pour recommencer encore
				//solution simple
			} else {

				//Soucis avec la recuperation deplusieurs caracteres sur la console, rien ne s'affiche
				//printf("vroum sur le clavier \n\r");

				//pseudo asservisssement à 45deg
				MPU6050_Read_All(&hi2c1, &mpu);
				load_pwm(htim3, k);
				true_angle = 90 - mpu.KalmanAngleX;
				printf("beforeangle %.2f \n\r", true_angle);
				HAL_Delay(50);

				while (true_angle < 45.00) {
					HAL_Delay(40);
					MPU6050_Read_All(&hi2c1, &mpu);
					true_angle = 90 - mpu.KalmanAngleX;
					printf("angle %.2f \n\r", true_angle);
					load_pwm(htim3, k);
					k++;
				};
				while (true_angle > 48.00) {
					HAL_Delay(40);
					true_angle = 90 - mpu.KalmanAngleX;
					MPU6050_Read_All(&hi2c1, &mpu);
					printf("angle %.2f \n\r", true_angle);
					load_pwm(htim3, k);
					k--;
				};

				//r_buffer[0] = ' ';
				etat = auto_mode;

				break;
			}

		case manual_mode:

			printf("State: Manual mode \n\r");

			//
			//recuperation de la pwm

			do {

				if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 2, 1)
						== HAL_OK)
					HAL_Delay(10);
				//HAL_UART_Transmit(&huart2, (uint8_t*) r_buffer, 2, 10);
				//HAL_Delay(50);
				valeur_can = load_adc(hadc1, 5);
				mapped_value = mapping_adc_value(valeur_can);
				HAL_Delay(100);
				load_pwm(htim3, mapped_value);

			} while (r_buffer[0] != '6');

			etat = motor_ready;
			r_buffer[0] = ' ';
			break;

		case init_pot:
			printf("State: Init pot \n\r");
			HAL_ADC_Start(&hadc1);
			//if (HAL_UART_Transmit(&huart2, (uint8_t*) "Auto mode \n\r", 15, 100)
			//	!= HAL_OK)
			//Error_Handler();
			okay = 1;
			//val = load_adc(hadc1, 5);
			//load_pwm(htim3, val);
			printf(">mettez le potentiometre à zero \n\r");
			//HAL_Delay(3000);

			while (okay == 1) {

				if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
					valeur_can = HAL_ADC_GetValue(&hadc1);
				}
				//valeur_can = load_adc(hadc1, 5);
				printf("Valeur can %d \n\r", valeur_can);
				//printf("\n");
				//mapped_value = mapping_adc_value(load_adc(hadc1, 5));
				//printf("pot %d \r", mapped_value);
				//printf("%d okayy \n \r",okay);
				//load_pwm(htim3, mapped_value);
				if (mapped_value <= 1513) {
					//okay = 0;
					//printf("o %d \n \r", okay);

				}
				HAL_Delay(50);
			}
			printf("succes \n\r");
			etat = manual_mode;
			r_buffer[0] = ' ';

			break;

		default:
			break;

		}

		//---------changement d'etat----FIN---

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
