/*
 *                                            ~| ECU Beta - Project |~
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Name: Johnathan Dekel
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 */
#include 	"ECU_Beta_main.h"
#include 	"stm32f7xx_hal.h"
#include  	<errno.h>
#include 	<ECU_Beta_lwip.h>
#include  	<sys/unistd.h>
#include  	<stdlib.h>
#include  	<stdio.h>
#include 	<string.h>

/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);
void BGR_GPIO_Init(void);
void BGR_TIMER6_Init(void);
void BGR_TIMER7_Init(void);
void BGR_USART3_UART_Init(void);
void BGR_LED_Management(uint8_t LED,uint32_t Value);
void BGR_CAN1_Init(void);
void BGR_CAN_Filter_Config(void);
void BGR_CAN1_Tx(uint8_t node,uint8_t Our_Message1[8]);
void BGR_Error_Handler(void);

/* Private variables ---------------------------------------------------------*/

//	SystemClock variables:
RCC_OscInitTypeDef 			RCC_OscInitStruct 			= {0};
RCC_ClkInitTypeDef 			RCC_ClkInitStruct 			= {0};
RCC_PeriphCLKInitTypeDef 	PeriphClkInitStruct 		= {0};

//	GPIO variables:
GPIO_InitTypeDef 			GPIO_InitStruct 			= {0};

//  B-Timer variables:
TIM_HandleTypeDef 			htimer6						= {0};
TIM_HandleTypeDef 			htimer7						= {0};
uint8_t						htimer7_flag				= {0};

//	UART variables:
UART_HandleTypeDef 			huart3 						= {0};
	char 					msg[100] 					= {0};
	int 					i 							= {0};

//	CAN1 variables:
CAN_HandleTypeDef 			hcan1						= {0};
CAN_RxHeaderTypeDef 		RxHeader1					= {0};
CAN_TxHeaderTypeDef 		TxHeader					= {0};
CAN_FilterTypeDef 			can1_filter_init			= {0};
CAN_FilterTypeDef 			can2_filter_init			= {0};
	uint8_t 				CanNode1 					= 2;
	uint32_t 				TxMailbox					= {0};
	uint8_t 				Our_Last_Message[8]			= {0};
	uint8_t 				Our_Message[8]				= {0};
	uint32_t 				Warning_flag				= {0};
	uint32_t                MINvalue 					= 500;
	uint32_t				Maxvalue 					= 1550;
	uint32_t 				Middle_1	 				= 1025;
	uint32_t				MINvalue2 					= 950;
	uint32_t				Maxvalue2 					= 1950;
	uint32_t 				Middle_2	 				= 1450;

//	CAN2 variables:
CAN_HandleTypeDef 			hcan2						= {0};
CAN_RxHeaderTypeDef 		RxHeader2					= {0};
	uint8_t 				CanNode2 					= 3;

//  Ethernet variables:
ETH_HandleTypeDef 			heth						= {0};
	ip4_addr_t 				destIPAddr					= {0};
	uint8_t 				Our_ETH_Message 			= {0};
	uint8_t 				STATUS                      = NEUTRAL;

	int flag = 0;
	uint8_t val  = 0;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                                      | Main |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	BGR_GPIO_Init();
	BGR_TIMER6_Init();
	BGR_TIMER7_Init();
	BGR_USART3_UART_Init();
	BGR_CAN1_Init();
	BGR_CAN_Filter_Config();
	MX_LWIP_Init();

		if( HAL_CAN_Start(&hcan1) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		if(HAL_CAN_ActivateNotification(&hcan1,CAN_IT_TX_MAILBOX_EMPTY|CAN_IT_RX_FIFO0_MSG_PENDING|CAN_IT_BUSOFF)!= HAL_OK)
	 	{
			BGR_Error_Handler();
	 	}

	  	if( HAL_CAN_Start(&hcan2) != HAL_OK)
	  	{
	  		BGR_Error_Handler();
	    }

	  	if(HAL_CAN_ActivateNotification(&hcan2,CAN_IT_TX_MAILBOX_EMPTY|CAN_IT_RX_FIFO1_MSG_PENDING|CAN_IT_BUSOFF)!= HAL_OK)
	    {
	    	BGR_Error_Handler();
	    }

	  	TxHeader.DLC 	= 8;
		TxHeader.StdId 	= 0x21;
		TxHeader.IDE   	= CAN_ID_STD;
		TxHeader.RTR 	= CAN_RTR_REMOTE;

		if( HAL_CAN_AddTxMessage(&hcan1,&TxHeader,Our_Message,&TxMailbox) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		if(Create_UDP_Sockets() != ERR_OK)
		{
			BGR_Error_Handler();
		}

		if(Start_Engines() != ERR_OK)
		{
			BGR_Error_Handler();
		}

		if(Ask_Engines() != ERR_OK)
		{
			BGR_Error_Handler();
		}
		BGR_LED_Management(STATUS,0);


	while (1)
	{
		MX_LWIP_Process();
		HAL_Delay(1);

	}

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                         | System Clock Configurations |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void SystemClock_Config(void)
{
  /** Configure LSE Drive Capability */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure. */
  RCC_OscInitStruct.OscillatorType 			= RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState 				= RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState 			= RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource 			= RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM	 			= 4;
  RCC_OscInitStruct.PLL.PLLN 				= 216;
  RCC_OscInitStruct.PLL.PLLP 				= RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ 				= 9;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
	  BGR_Error_Handler();
  }

  /** Activate the Over-Drive mode */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
	  BGR_Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks*/
  RCC_ClkInitStruct.ClockType	 			= RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource 			= RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider 			= RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider 			= RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider 			= RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
	  BGR_Error_Handler();
  }

  PeriphClkInitStruct.PeriphClockSelection 	= RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Usart3ClockSelection 	= RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection 	= RCC_CLK48SOURCE_PLL;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
	  BGR_Error_Handler();
  }

  /**Configure the Systick interrupt time*/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  /**Configure the Systick*/
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                            | GPIO Initialization |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_GPIO_Init(void)
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin 			= USER_Btn_Pin;
  GPIO_InitStruct.Mode 			= GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull 			= GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : relay_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin 			= relay_Pin|GPIO_PIN_0|GPIO_PIN_7|GPIO_PIN_14;
  GPIO_InitStruct.Mode 			= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 			= GPIO_NOPULL;
  GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PD13 */
  GPIO_InitStruct.Pin 			= GPIO_PIN_13;
  GPIO_InitStruct.Mode 			= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 			= GPIO_NOPULL;
  GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : R2D_Pin */
  GPIO_InitStruct.Pin 			= R2D_Pin;
  GPIO_InitStruct.Mode 			= GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull			= GPIO_NOPULL;
  HAL_GPIO_Init(R2D_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MinMax_Gas */
  GPIO_InitStruct.Pin 			= GPIO_PIN_4;
  GPIO_InitStruct.Mode 			= GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull 			= GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  //---------------------------------------------------

//Torque-LED's

   /*Configure GPIO pins : LED1, LED4, LED6 */
   GPIO_InitStruct.Pin 			= GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
   GPIO_InitStruct.Mode 			= GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull 			= GPIO_NOPULL;
   GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

   /*Configure GPIO pins : LED2, LED3, LED5, LED9, LED10 ,WARNING*/
   GPIO_InitStruct.Pin 			= GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_13|GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_10;
   GPIO_InitStruct.Mode 			= GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull 			= GPIO_NOPULL;
   GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

   /*Configure GPIO pins : LED7, LED8 */
   GPIO_InitStruct.Pin 			= GPIO_PIN_14|GPIO_PIN_9;
   GPIO_InitStruct.Mode 			= GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull 			= GPIO_NOPULL;
   GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

//Brakes-LED's

     /*Configure GPIO pins : LED1, LED4 */
  GPIO_InitStruct.Pin 			= GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode 			= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 			= GPIO_NOPULL;
  GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LED2 */
  GPIO_InitStruct.Pin 			= GPIO_PIN_2;
  GPIO_InitStruct.Mode 			= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 			= GPIO_NOPULL;
  GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LED3 */
  GPIO_InitStruct.Pin 			= GPIO_PIN_4;
  GPIO_InitStruct.Mode 			= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 			= GPIO_NOPULL;
  GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : LED5, LED6, LED7 */
  GPIO_InitStruct.Pin 			= GPIO_PIN_13|GPIO_PIN_12|GPIO_PIN_11;
  GPIO_InitStruct.Mode 			= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 			= GPIO_NOPULL;
  GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : LED8, LED10 */
  GPIO_InitStruct.Pin 			= GPIO_PIN_2|GPIO_PIN_0;
  GPIO_InitStruct.Mode 			= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 			= GPIO_NOPULL;
  GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : LED9 */
  GPIO_InitStruct.Pin 			= GPIO_PIN_0;
  GPIO_InitStruct.Mode 			= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 			= GPIO_NOPULL;
  GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  //---------------------------------------------------

  HAL_NVIC_EnableIRQ(EXTI4_IRQn);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                           | Basic Timer Initialization |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_TIMER6_Init(void)
{
	htimer6.Instance 		= TIM6;

//   5ms

//	htimer6.Init.Prescaler 	= 50;
//	htimer6.Init.Period 	= 10591;

//         || TEST ||

//   1ms

//	htimer6.Init.Prescaler 	= 10;
//	htimer6.Init.Period 	= 10591;

//   3ms

//	htimer6.Init.Prescaler 	= 30;
//	htimer6.Init.Period 	= 10591;

//   100ms

	htimer6.Init.Prescaler 	= 1000;
	htimer6.Init.Period 	= 10591;

 //  0.5sec

//	htimer6.Init.Prescaler 	= 5000;
//	htimer6.Init.Period 	= 10591;


	if( HAL_TIM_Base_Init(&htimer6) != HAL_OK )
	{
		BGR_Error_Handler();
	}
}

void BGR_TIMER7_Init(void)
{
	htimer7.Instance 		= TIM7;

//  2sec

	htimer7.Init.Prescaler 	= 7500;
	htimer7.Init.Period 	= 21597-1;

	if( HAL_TIM_Base_Init(&htimer7) != HAL_OK )
	{
		BGR_Error_Handler();
	}
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                               | UART - Initialization |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_USART3_UART_Init(void)
{
  huart3.Instance 						= USART3;
  huart3.Init.BaudRate 					= 115200;
  huart3.Init.WordLength 				= UART_WORDLENGTH_8B;
  huart3.Init.StopBits 					= UART_STOPBITS_1;
  huart3.Init.Parity 					= UART_PARITY_NONE;
  huart3.Init.Mode 						= UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl 				= UART_HWCONTROL_NONE;
  huart3.Init.OverSampling 				= UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling 			= UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit 	= UART_ADVFEATURE_NO_INIT;

  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
	  BGR_Error_Handler();
  }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                               | GPIO - LED Management |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_LED_Management(uint8_t LED,uint32_t Value)
{

	switch(LED)
	{
		case NEUTRAL:

			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
			break;

		case DRIVE:

			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
			break;

		case TRAINER:

			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
			break;

		case Torques_LED:

			HAL_GPIO_WritePin(GPIOF,GPIO_PIN_13,GPIO_PIN_RESET); //Torques LED1
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_RESET);	 //Torques LED2
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET); //Torques LED3
			HAL_GPIO_WritePin(GPIOF,GPIO_PIN_14,GPIO_PIN_RESET); //Torques LED4
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_RESET); //Torques LED5
			HAL_GPIO_WritePin(GPIOF,GPIO_PIN_15,GPIO_PIN_RESET); //Torques LED6
			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_RESET); //Torques LED7
			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_RESET);  //Torques LED8
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_RESET);  //Torques LED9
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_RESET);  //Torques LED10

			if(TORQUE_LED1_LOW < Value)
			{
				HAL_GPIO_WritePin(GPIOF,GPIO_PIN_13,GPIO_PIN_SET); //LED1
			}

			if(TORQUE_LED2_LOW < Value)
			{
				HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_SET);	 //LED2
			}

			if(TORQUE_LED3_LOW < Value)
			{
				HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET); //LED3
			}

			if(TORQUE_LED4_LOW < Value)
			{
				HAL_GPIO_WritePin(GPIOF,GPIO_PIN_14,GPIO_PIN_SET); //LED4
			}

			if(TORQUE_LED5_LOW < Value)
			{
				HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_SET); //LED5
			}

			if(TORQUE_LED6_LOW < Value)
			{
				HAL_GPIO_WritePin(GPIOF,GPIO_PIN_15,GPIO_PIN_SET); //LED6
			}

			if(TORQUE_LED7_LOW < Value)
			{
				HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_SET); //LED7
			}

			if(TORQUE_LED8_LOW < Value)
			{
				HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_SET);  //LED8
			}

			if(TORQUE_LED9_LOW < Value)
			{
				HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_SET);  //LED9
			}

			if(TORQUE_LED10_LOW < Value)
			{
				HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET);  //LED10
			}

			break;

		case Brakes_LED:

			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);  //Brakes LED1
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET);  //Brakes LED2
			HAL_GPIO_WritePin(GPIOF,GPIO_PIN_4,GPIO_PIN_RESET);  //Brakes LED3
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET);  //Brakes LED4
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_RESET); //Brakes LED5
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET); //Brakes LED6
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11,GPIO_PIN_RESET); //Brakes LED7
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET);  //Brakes LED8
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);  //Brakes LED9
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_RESET);  //Brakes LED10

			if(BRAKES_LED1_LOW < Value)
			{
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);  //LED1
			}

			if(BRAKES_LED2_LOW < Value)
			{
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);  //LED2
			}

			if(BRAKES_LED3_LOW < Value)
			{
				HAL_GPIO_WritePin(GPIOF,GPIO_PIN_4,GPIO_PIN_SET);  //LED3
			}

			if(BRAKES_LED4_LOW < Value)
			{
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);  //LED4
			}

			if(BRAKES_LED5_LOW < Value)
			{
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET); //LED5
			}

			if(BRAKES_LED6_LOW < Value)
			{
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET); //LED6
			}

			if(BRAKES_LED7_LOW < Value)
			{
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11,GPIO_PIN_SET); //LED7
			}

			if(BRAKES_LED8_LOW < Value)
			{
				HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET);  //LED8
			}

			if(BRAKES_LED9_LOW < Value)
			{
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);  //LED9
			}

			if(BRAKES_LED10_LOW < Value)
			{
				HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_SET);  //LED10
			}

			break;

		case WARNING:

			switch(Value)
			{
				case 1:

					HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET);
					break;

				case 2:

					HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_RESET);
					break;
			}

			break;

		case SHUTDOWN:

			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_RESET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_RESET);
			HAL_Delay(100);

			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
			break;

	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                              | Interrupt Management |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	uint8_t msg[8];
	uint8_t msg_check;


	  if(htim->Instance == TIM6)
	  {

		   BGR_CAN1_Tx(CanNode1,msg);
//		   HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
//		   HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);
//		   HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_14);
	  }

	  if(htim->Instance == TIM7)
	  {
		  if (htimer7_flag == 0)
		  {
			  htimer7_flag = 1;
		  }
		  else
		  {
			  BGR_LED_Management(SHUTDOWN,0);
			  STATUS = NEUTRAL;
			  msg_check = STATUS + 2;
			  BGR_CAN1_Tx(msg_check ,msg);
			  HAL_TIM_Base_Stop_IT(&htimer7);
		  }
	  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	uint8_t rcvd_msg[8] 	= {0};
	uint32_t Accelerator_1  = {0};
	uint32_t Accelerator_2 	= {0};

	switch(GPIO_Pin)
	{

	case GPIO_PIN_4:

		if (STATUS != NEUTRAL)
			return;

		if(HAL_CAN_GetRxMessage(&hcan1,CAN_RX_FIFO0,&RxHeader1,rcvd_msg) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		if (RxHeader1.StdId != 0x125)
			return;

		Accelerator_1 = (Our_Last_Message[0] << 8) + Our_Last_Message[1];
		Accelerator_2 = (Our_Last_Message[2] << 8) + Our_Last_Message[3];

		if(Accelerator_1 > Middle_1)
		{
			Maxvalue = Accelerator_1;
		}

		else
		{
			MINvalue = Accelerator_1;
		}

		if(Accelerator_2 > Middle_2)
		{
			Maxvalue2 = Accelerator_2;
		}

		else
		{
			MINvalue2 = Accelerator_2;
		}

		break;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                                  | Error Handler |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_Error_Handler(void)
{
//	while(1);
}
