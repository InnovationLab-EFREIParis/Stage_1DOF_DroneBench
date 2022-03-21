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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include"yann.h"
#include "remi.h"
#include<stdio.h>
//#include<stdbool.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim3;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM3_Init(void);
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
	 char gaz_data[4];
	 	int counter=4;



	//char buffer [size];
	//int k = 0;
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
  /* USER CODE BEGIN 2 */
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
	y_print(&huart2, " 0 to 6 to change state \r\n", 26);
	HAL_Delay(3000);

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
			printf("nucleo ready\r\n");
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
			printf("Info mode\r\n");
			printf("Firmware version %.2f \n\r", firmware_version);
			HAL_Delay(3000);

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
			printf("Motor Initialization \n\r");
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
			//k = 0;
			load_pwm(htim3, valeur_min_moteur);
			printf("Motor ready \n\r");
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
			printf("Auto mode \n\r");

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
			}else {

				//Soucis avec la recuperation deplusieurs caracteres sur la console, rien ne s'affiche
				printf("vroum sur le clavier \n\r");
				HAL_Delay(1150);

				//clear le gaz buffer
				gaz_buffer[0]=' ',gaz_buffer[1]=' ',gaz_buffer[2]=' ',gaz_buffer[3]=' ';
				do {


						if (HAL_UART_Receive(&huart2, (uint8_t*) gaz_buffer, 4, 10)
														== HAL_OK)
						HAL_UART_Transmit(&huart2, (uint8_t*) gaz_buffer, 4, 10);


						//je compare les differentes case de mon tableau pour ma boucle de sortie
						} while (gaz_buffer[1] !='\n' && gaz_buffer[2] !='\n' && gaz_buffer[3] !='\n' );
				etat = auto_mode;
				break;
		}





		case manual_mode:

			if (HAL_UART_Transmit(&huart2, (uint8_t*) "Manual mode \n\r", 15,
					100) != HAL_OK)
				Error_Handler();
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
			//if (HAL_UART_Transmit(&huart2, (uint8_t*) "Auto mode \n\r", 15, 100)
			//	!= HAL_OK)
			//Error_Handler();
			okay = 1;
			//val = load_adc(hadc1, 5);
			//load_pwm(htim3, val);
			printf("mettez le potentiometre à zero \n\r");
			//HAL_Delay(3000);

			while (okay == 1) {
				//valeur_can = load_adc(hadc1, 5);
				//printf("%d \r",valeur_can);
				//printf("\n");
				mapped_value = mapping_adc_value(load_adc(hadc1, 5));
				printf("pot %d \r", mapped_value);
				//printf("%d okayy \n \r",okay);
				//load_pwm(htim3, mapped_value);
				if (mapped_value <= 1513) {
					okay = 0;
					//printf("o %d \n \r", okay);

				}

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

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 4096;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 25;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
  /* DMA1_Channel7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

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

