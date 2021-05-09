/*
                                            			 ~| Pedals |~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Name: Johnathan Dekel                                                                          last time updated: 25/10/20

   New_Pedals_main.c


  Table of Contents:

  1.  Headline
  2.  main
  3.  System Clock Configurations
  4.  GPIO Initialization
  5.  ADC Initialization
  6.  ADC Data Receiving
  7.  UART Initialization
  8.  CAN Initialization
  9.  CAN Filter Configurations
  10. CAN Message Transmitting
  11. CAN Message Receiving
  12. Interrupt Management
  13. Error Handler

                                                                                                                           */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 1.                                                     | Headline |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include 	<New_Pedals_main.h>
#include 	<string.h>
#include 	<stdio.h>

/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);
void BGR_GPIO_Init(void);
void BGR_ADC2_Init(void);

void BGR_CAN_Init(void);
void BGR_CAN_Filter_Config(void);
void BGR_CAN_Tx(void);
void BGR_CAN_Rx(void);
int  BGR_ADC2_Rx(void);
void BGR_USART2_UART_Init(void);
void BGR_Error_Handler();

/* Private variables ---------------------------------------------------------*/

//	SystemClock variables:
RCC_OscInitTypeDef 		 	RCC_OscInitStruct 		= {0};
RCC_ClkInitTypeDef 		 	RCC_ClkInitStruct		= {0};
RCC_PeriphCLKInitTypeDef 	PeriphClkInit 	    	= {0};

//	GPIO variables:
GPIO_InitTypeDef 		 	GPIO_InitStruct 		= {0};

//	ADC variables:
ADC_HandleTypeDef 		 	hadc2 					= {0};
ADC_ChannelConfTypeDef 		sConfig  				= {0};
	uint16_t 				ADC_DATA1_Raw 			= {0};
	uint16_t 				ADC_DATA2_Raw 			= {0};
	int16_t  				ADC_DATA3_Raw 			= {0};

//	CAN variables:
CAN_HandleTypeDef 			hcan 					= {0};
CAN_FilterTypeDef 			can_filter_init 		= {0};
CAN_TxHeaderTypeDef 		TxHeader 				= {0};
CAN_RxHeaderTypeDef 		RxHeader 				= {0};
	uint32_t 				TxMailbox 				= {0};
	uint8_t 				Our_Message[8] 			= {0};
	uint8_t 				rcvd_msg[8] 			= {0};

//	UART variables:
UART_HandleTypeDef 			huart2 					= {0};
	char 					msg[100] 				= {0};
	int 					High_Value_Registered 	= {0};
	int 					Low_Value_Registered 	= {0};
	uint16_t 				High_Value 				= {0};
	uint16_t 				Low_Value 				= {0};


//	Error Handler variables:
	uint8_t 				Error_Number 			= {0};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 2.                                                     | main |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  BGR_USART2_UART_Init();
  BGR_GPIO_Init();
  BGR_ADC2_Init();
  BGR_CAN_Init();
  BGR_CAN_Filter_Config();

  	  if( HAL_CAN_Start(&hcan) != HAL_OK)
  	  {
  		  Error_Number = 11;
  		  BGR_Error_Handler();
  	  }

  	  if(HAL_CAN_ActivateNotification(&hcan,CAN_IT_TX_MAILBOX_EMPTY|CAN_IT_RX_FIFO0_MSG_PENDING|CAN_IT_BUSOFF)!= HAL_OK)
	  {
  		  Error_Number = 12;
		  BGR_Error_Handler();
	  }

  	  //HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_3);

  while (1)
  {
	  //Timing Test: Set GPIO pin high
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);


	  	  //Configure ADC_CH1
	  	  sConfig.Channel 	= ADC_CHANNEL_1;

	  	  //Get ADC_CH1 value
	  	  ADC_DATA1_Raw 	= BGR_ADC2_Rx();

	  	  //Configure ADC_CH2
	  	  sConfig.Channel 	= ADC_CHANNEL_2;

	  	  //Get ADC_CH2 value
	  	  ADC_DATA2_Raw 	= BGR_ADC2_Rx();

	  	  //Configure ADC_CH3
	  	  sConfig.Channel 	= ADC_CHANNEL_3;

	  	  //Get ADC_CH3 value
	  	  ADC_DATA3_Raw 	= BGR_ADC2_Rx();


	  //Timing Test: Set GPIO pin low
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);

//	  if (High_Value_Registered)
//	  {
//		  sprintf(msg,"High Value :  0x%x\r\n",High_Value);
//		  HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);
//	  }
//	  else
//	  {
//		  sprintf(msg,"High Value was not registered!\r\n");
//		  HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);
//	  }
//
//	  if (Low_Value_Registered)
//	  {
//		  sprintf(msg,"Low Value :  0x%x\r\n",Low_Value);
//		  HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);
//	  }
//	  else
//	  {
//		  sprintf(msg,"Low Value was not registered!\r\n");
//		  HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);
//	  }
//
//	  sprintf(msg,"\n-----------------------------\r\n\n");
//	  HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);
//
//	  //Convert to string and print
//	  sprintf(msg,"CH1  (PA4-A3) Sample:  0x%x\r\n",ADC_DATA1_Raw);
//	  HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);
//
//	  sprintf(msg,"CH2  (PA5-A4) Sample:  0x%x\r\n",ADC_DATA2_Raw);
//	  HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);
//
//
//	  sprintf(msg,"CH3  (PA6-A5) Sample:  0x%x\r\n",ADC_DATA3_Raw);
//	  HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);
//
//	  sprintf(msg,"\n-----------------------------\r\n\n");
//	  HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);
//	  //pretend we have something else to do for a while
//	  HAL_Delay(1);
  }
  return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 3.                                        | System Clock Configurations |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void SystemClock_Config(void)
{
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.  */

  RCC_OscInitStruct.OscillatorType 		= RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState 			= RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState 		= RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource 		= RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL 			= RCC_PLL_MUL16;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
	  Error_Number = 1;
	  BGR_Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks  */
  RCC_ClkInitStruct.ClockType 		= RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource 	= RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider	= RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
	  Error_Number = 2;
	  BGR_Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection 	= RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Adc12ClockSelection 	= RCC_ADC12PLLCLK_DIV1;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
	  Error_Number = 3;
	  BGR_Error_Handler();
  }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 4.                                           | GPIO - Initialization |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_GPIO_Init(void)
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD2_Pin|LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LD2_Pin LD3_Pin */
  GPIO_InitStruct.Pin 	= LD2_Pin|LD3_Pin;
  GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 	= GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin 	= USER_Btn_Pin;
  GPIO_InitStruct.Mode 	= GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull 	= GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 5.                                           | ADC - Initialization |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_ADC2_Init(void)
{
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler 		= ADC_CLOCK_ASYNC_DIV1;
  hadc2.Init.Resolution 			= ADC_RESOLUTION_12B;
  hadc2.Init.ScanConvMode 			= ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode 	= DISABLE;
  hadc2.Init.DiscontinuousConvMode 	= DISABLE;
  hadc2.Init.ExternalTrigConvEdge 	= ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.ExternalTrigConv 		= ADC_SOFTWARE_START;
  hadc2.Init.DataAlign 				= ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion 		= 1;
  hadc2.Init.DMAContinuousRequests 	= DISABLE;
  hadc2.Init.EOCSelection 			= ADC_EOC_SINGLE_CONV;
  hadc2.Init.LowPowerAutoWait 		= DISABLE;
  hadc2.Init.Overrun 				= ADC_OVR_DATA_OVERWRITTEN;

  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
	  Error_Number = 4;
	  BGR_Error_Handler();
  }

  sConfig.Channel 		= ADC_CHANNEL_1;
  sConfig.Rank 			= ADC_REGULAR_RANK_1;
  sConfig.SingleDiff 	= ADC_SINGLE_ENDED;
  sConfig.SamplingTime 	= ADC_SAMPLETIME_1CYCLE_5;
  sConfig.OffsetNumber 	= ADC_OFFSET_NONE;
  sConfig.Offset 		= 0;

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 6.                            	            | ADC - Data Receiving |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int BGR_ADC2_Rx(void)
{
	uint16_t ADC_DATA;

	if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
	{
		Error_Number = 13;
		BGR_Error_Handler(Error_Number);
	}
		// Get ADC_CHx value
        if (HAL_ADC_Start(&hadc2) != HAL_OK)
	    {
        	Error_Number = 14;
        	BGR_Error_Handler();
		}

        if ( HAL_ADC_PollForConversion(&hadc2,HAL_MAX_DELAY) != HAL_OK)
        {
        	Error_Number = 15;
        	BGR_Error_Handler();
        }

		ADC_DATA = HAL_ADC_GetValue(&hadc2);

	return ADC_DATA;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  7.                                          | UART - Initialization |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_USART2_UART_Init(void)
{
  huart2.Instance 						= USART2;
  huart2.Init.BaudRate 					= 38400;
  huart2.Init.WordLength 				= UART_WORDLENGTH_8B;
  huart2.Init.StopBits 					= UART_STOPBITS_1;
  huart2.Init.Parity 					= UART_PARITY_NONE;
  huart2.Init.Mode 						= UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl 				= UART_HWCONTROL_NONE;
  huart2.Init.OverSampling 				= UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling 			= UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit 	= UART_ADVFEATURE_NO_INIT;

  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
	  Error_Number = 5;
	  BGR_Error_Handler();
  }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 8.                                           | CAN - Initialization |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_CAN_Init(void)
{
	hcan.Instance 					= CAN;
	hcan.Init.Mode 					= CAN_MODE_NORMAL;
	hcan.Init.AutoBusOff 			= ENABLE;
	hcan.Init.AutoRetransmission 	= DISABLE;
	hcan.Init.AutoWakeUp 			= ENABLE;
	hcan.Init.ReceiveFifoLocked 	= DISABLE;
	hcan.Init.TimeTriggeredMode 	= DISABLE;
	hcan.Init.TransmitFifoPriority 	= DISABLE;

	//Settings related to CAN bit timings  - http://www.bittiming.can-wiki.info/
	hcan.Init.Prescaler 			= 2;
	hcan.Init.SyncJumpWidth 		= CAN_SJW_1TQ;
	hcan.Init.TimeSeg1 				= CAN_BS1_13TQ;
	hcan.Init.TimeSeg2 				= CAN_BS2_2TQ;

	if ( HAL_CAN_Init (&hcan) != HAL_OK)
	{
		Error_Number = 6;
		BGR_Error_Handler();
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 9.                                       | CAN - Filter Configurations |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_CAN_Filter_Config(void)
{
	can_filter_init.FilterActivation 		= ENABLE;
	can_filter_init.FilterFIFOAssignment 	= CAN_RX_FIFO0;
	can_filter_init.FilterMode 				= CAN_FILTERMODE_IDMASK;
	can_filter_init.FilterScale 			= CAN_FILTERSCALE_32BIT;
	can_filter_init.SlaveStartFilterBank 	= 0;

/*	We need to mask only the first 11 MSB of register "FilterIdHigh" to filter the address:

	|		   FilterIdHigh (16 bit)			|				            FilterIdLow (16 bit)				   |
	| STID[10:0] (11 bit) | EXID[17:13] (5 bit) | EXID[12:0] (13 bit) |IDE[0] (1 bit) | RTR[0] (1 bit) | 0 (1 bit) |

	Note: even though FilterIdHigh/FilterIdLow are 32 bit, each one of them contains data of 16 bit.						*/

		can_filter_init.FilterBank  		= 0;					// Filter number 0.
		can_filter_init.FilterIdHigh 		= 0x2400;				// id_0x120 - 00100100000|00000
		can_filter_init.FilterIdLow 		= 0x0000;
		can_filter_init.FilterMaskIdHigh 	= 0XFFE0;          // mask    - 11111111111|00000
		can_filter_init.FilterMaskIdLow 	= 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan,&can_filter_init) != HAL_OK)
		{
			Error_Number = 7;
			BGR_Error_Handler();
		}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 10.                                        | CAN - Message Transmitting |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_CAN_Tx(void)
{
	// the content of the massage - each block is 1 byte
	Our_Message[0] 	= (ADC_DATA1_Raw >> 8);
	Our_Message[1] 	=  ADC_DATA1_Raw & 0xff;
	Our_Message[2] 	= (ADC_DATA2_Raw >> 8);
	Our_Message[3] 	=  ADC_DATA2_Raw & 0xff;
	Our_Message[4] 	= (ADC_DATA3_Raw >> 8);
	Our_Message[5] 	=  ADC_DATA3_Raw & 0xff;
	Our_Message[6] 	= (0 >> 8);
	Our_Message[7] 	=  0 & 0xff;

	TxHeader.DLC 	= 6;
	TxHeader.StdId 	= 0x125;
	TxHeader.IDE   	= CAN_ID_STD;
	TxHeader.RTR	= CAN_RTR_DATA;

	if( HAL_CAN_AddTxMessage(&hcan,&TxHeader,Our_Message,&TxMailbox) != HAL_OK)
	{
		Error_Number = 17;
		BGR_Error_Handler();
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 11.                                          | CAN - Message Receiving |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_CAN_Rx(void)
{
	if( HAL_CAN_GetRxMessage(&hcan,CAN_RX_FIFO0,&RxHeader,rcvd_msg) != HAL_OK)
	{
		Error_Number = 16;
		BGR_Error_Handler();
	}

	if(RxHeader.StdId == 0x120)
	{
		BGR_CAN_Tx();
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 12.                                            | Interrupt Management |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	BGR_CAN_Rx();
	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_3);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_3);

	if(ADC_DATA1_Raw > 2048)
	{
		High_Value_Registered 	= 1;
		High_Value 				= ADC_DATA1_Raw;
	}

	else
	{
		Low_Value_Registered 	= 1;
		Low_Value 				= ADC_DATA1_Raw;
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 13.                                                 | Error Handler |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_Error_Handler()
{
	while(1);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~| FIN |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
