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
 * 	scl=PB8
 * 	sda = PB9
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
	double true_angle;

	int pwm_auto_mode = valeur_min_moteur;	//var used to increement speed in auto state

	int cpt_char = 0;
	int max_cpt_char = 4;
	char r_buffer_string[max_cpt_char];
	int gaz_term_percent = 0;
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
	while (MPU6050_Init(&hi2c1) == 1)
		;
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
		// Reinitialisation du buffer
		r_buffer[0] = 0;
		//differents etats qu'on peut avoir

		//---------changement d'etat-------

		switch (etat) {

		case init_uc:
			//traitement des sorties
			printf("State: Init uc\n\r");
			printf("> Press 1 for Motor ready\n\r");
			printf("> Press 2 for info mode\n\r");

			//gyro init
			//MPU6050_Read_All(&hi2c1, &mpu);
			//HAL_Delay(1000);
			//traitement des entrées (transitions)
			do {
				HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 2, 10);
			} while ((r_buffer[0] != '1') && (r_buffer[0] != '2'));

			switch (r_buffer[0]) {
			case '1':
				etat = motor_ready;
				break;
			case '2':
				etat = info_mode;
				break;
			default:
				break;
			}

			// Reinitialisation du buffer
			//r_buffer[0] = 0;
			break;

		case info_mode:
			printf("State: Info mode\n\r");
			printf(">Firmware version %.2f \n\r", firmware_version);
			printf(">Baudrate %lu \n\r", huart2.Init.BaudRate);
			printf(">States: \n\r");
			printf(">>0 Init uc: Init Microcontroller \n\r");
			printf(">>2 Info Mode \n\r");
			printf(">>3 Init Pot \n\r");
			printf(">>4 Auto Mode \n\r");
			printf(">>5 Manual Mode Pot \n\r");
			printf(">>6 Moto Ready Mode \n\r");
			printf(">>7 Manual Mode Term\n\r");

			//go to init
			etat = init_uc;
			// Reinitialisation du buffer
			//r_buffer[0] = 0;
			break;

		case motor_ready:
			printf("State: Motor ready \n\r");
			printf("> Press 2 for info mode\n\r");
			printf("> Press 3 for init pot\n\r");
			printf("> Press 4 for auto mode\n\r");
			printf("> Press 7 for manual mode term\n\r");

			// Init Motor
			load_pwm(htim3, valeur_min_moteur);
			HAL_Delay(100);

			//k = 1700;
			//load_pwm(htim3, valeur_min_moteur);
			//HAL_Delay(100);
			do {

				if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 2, 10)
						== HAL_OK) {
					HAL_Delay(50);
				}
			} while ((r_buffer[0] != '2') && (r_buffer[0] != '3')
					&& (r_buffer[0] != '4') && (r_buffer[0] != '7'));

			switch (r_buffer[0]) {
			case '2':
				etat = info_mode;
				break;
			case '3':
				etat = init_pot;
				break;
			case '4':
				etat = auto_mode;
				break;
			case '7':
				etat = manual_mode_term;
				break;
			default:
				break;
			}

			break;

		case auto_mode:
			printf("State: Auto mode \n\r");
			printf("> Press 6 for motor ready mode\n\r");
			if (MPU6050_Init(&hi2c1) == 0) {
				printf("> Gyro MPU6050 initialized\n\r");
			} else {
				printf("> Gyro MPU6050 not working\n\r");
				etat = motor_ready;
				break;
			}

			do {
				if (HAL_UART_Receive(&huart2, (uint8_t*) r_buffer, 2, 10)
						== HAL_OK) {
				}
				//pseudo asservisssement à 45deg
				MPU6050_Read_All(&hi2c1, &mpu);
				//load_pwm(htim3, k);

				true_angle = 90 - mpu.KalmanAngleX;
				printf("%.2f\n\r", true_angle);
				HAL_Delay(50);

				while (true_angle < 20.00) {
					HAL_Delay(50);
					MPU6050_Read_All(&hi2c1, &mpu);
					true_angle = 90 - mpu.KalmanAngleX;
					printf("%.2f\n\r", true_angle);
					load_pwm(htim3, pwm_auto_mode);
					pwm_auto_mode++;
				};
				while (true_angle > 25.00) {
					HAL_Delay(50);
					true_angle = 90 - mpu.KalmanAngleX;
					MPU6050_Read_All(&hi2c1, &mpu);
					printf("%.2f\n\r", true_angle);
					load_pwm(htim3, pwm_auto_mode);
					pwm_auto_mode--;
				};
			} while (r_buffer[0] != '6');
			etat = motor_ready;
			break;

		case manual_mode_pot:
			printf("State: Manual mode pot \n\r");
			printf("> Press 6 for motor ready mode\n\r");
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
				//load_pwm_filtre(htim3, mapped_value);

			} while (r_buffer[0] != '6');

			etat = motor_ready;
			// Reinitialisation du buffer
			r_buffer[0] = 0;
			break;

		case manual_mode_term:
			printf("State: Manual mode term\n\r");
			printf(
					"> Enter value between 1 and 100 (power pourcentage) then presse enter\n\r");
			printf("> Enter + or -\n\r");
			printf("> Enter 0 then press enter for Motor ready\n\r");
			//recuperation de la pwm
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
					&& (r_buffer[0] != '-'));
			//HAL_UART_Transmit(&huart2, (uint8_t*) r_buffer_string, max_cpt_char,
			//		10);
			//HAL_UART_Transmit(&huart2, "\n\r", 2, 10);

			int value0 = r_buffer_string[0] - '0';
			int value1 = r_buffer_string[1] - '0';
			int value2 = r_buffer_string[2] - '0';

			//printf("Cpt_char %d\n\r", cpt_char);
			if (cpt_char == 2) {
				gaz_term_percent = value0;
			}
			if (cpt_char == 3) {
				gaz_term_percent = value0 * 10 + value1;
			}
			if (cpt_char == 4) {
				gaz_term_percent = value0 * 100 + value1 * 10 + value2;
			}
			if (r_buffer[0] == '+') {
				gaz_term_percent++;
			}
			if (r_buffer[0] == '-') {
				gaz_term_percent--;
			}

			printf("Gaz Term %d \n\r", gaz_term_percent);

			mapped_value = mapping_adc_value_percent(gaz_term_percent);

			printf("Mapping adc value percent %d\n\r", mapped_value);
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
			break;

		case init_pot:
			printf("State: Init pot \n\r");
			HAL_ADC_Start(&hadc1);
			okay = 1;
			//val = load_adc(hadc1, 5);
			//load_pwm(htim3, val);
			printf(">mettez le potentiometre a zero \n\r");
			HAL_Delay(100);

			while (okay == 1) {
				HAL_ADC_Start(&hadc1);
				if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
					valeur_can = HAL_ADC_GetValue(&hadc1);
					printf(">Valeur can :\t %d \n\r", valeur_can);
				} else {

					printf(">Convertion NOK\n\r");
				}
				//printf("\n");
				//mapped_value = mapping_adc_value(load_adc(hadc1, 5));
				//printf("pot %d \r", mapped_value);
				//printf("%d okayy \n \r",okay);
				//load_pwm(htim3, mapped_value);
				if (valeur_can <= 20) {
					okay = 0;
					//printf("o %d \n \r", okay);

				}
				HAL_Delay(1000);

			}
			printf("\n succes \n\r");
			etat = manual_mode_pot;
			// Reinitialisation du buffer
			r_buffer[0] = 0;

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
