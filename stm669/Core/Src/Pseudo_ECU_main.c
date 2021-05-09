/*
                                            			 ~| Pseudo ECU |~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Name: Johnathan Dekel                                                                          last time updated: 25/10/20

	Pseudo_ECU_main.c


  Table of Contents:

  1.  Headline
  2.  main
  3.  System Clock Configurations
  4.  GPIO Initialization
  5.  LED Management
  6.  Basic Timer Initialization
  7.  CAN Initialization
  8.  CAN Filter Configurations
  9.  CAN Message Transmitting
  10. CAN Message Receiving
  11. Interrupt Management
  12. Error Handler

                                                                                                                           */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 1.                                                     | Headline |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <Pseudo_ECU_main.h>
#include "stm32f4xx_hal.h"

/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config_HSE(uint8_t clock_freq);
void BGR_TIMER6_Init(void);
void BGR_GPIO_Init(void);
void BGR_Error_Handler(void);
void BGR_CAN1_Init(void);
void BGR_CAN_Filter_Config(void);
void BGR_CAN1_Tx(uint8_t node,uint8_t Our_Message[8]);
void BGR_CAN1_Rx(void);
void BGR_LED_Manage_Output(uint8_t led_no);

/* Private variables ---------------------------------------------------------*/

//	SystemClock variables:
RCC_OscInitTypeDef RCC_OscInitStruct = {0};
RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

//	GPIO variables:
GPIO_InitTypeDef GPIO_InitStruct = {0};
	uint8_t led_num = {0};

//	Basic Timer variables:
TIM_HandleTypeDef htimer6 = {0};

//	CAN variables:
CAN_HandleTypeDef hcan1 = {0};
CAN_FilterTypeDef can1_filter_init = {0};
CAN_TxHeaderTypeDef TxHeader = {0};
CAN_RxHeaderTypeDef RxHeader = {0};
	uint32_t TxMailbox = {0};
	uint8_t CanNode = 2;
	uint8_t rcvd_msg[8] = {0};
	uint8_t msg_check = 1;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 2.                                                     | main |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main(void)
{
	HAL_Init();
	SystemClock_Config_HSE(SYS_CLOCK_FREQ_180_MHZ);
	BGR_GPIO_Init();
	BGR_TIMER6_Init();
	BGR_CAN1_Init();
	BGR_CAN_Filter_Config();

	 if( HAL_CAN_Start(&hcan1) != HAL_OK)
		{
		 BGR_Error_Handler();
		}
	 if(HAL_CAN_ActivateNotification(&hcan1,CAN_IT_TX_MAILBOX_EMPTY|CAN_IT_RX_FIFO0_MSG_PENDING|CAN_IT_BUSOFF)!= HAL_OK)
	 	{
		 BGR_Error_Handler();
	 	}

	while(1);

	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 3.                                        | System Clock Configurations |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void SystemClock_Config_HSE(uint8_t clock_freq)
{
	  /** Configure the main internal regulator output voltage	  */
	  __HAL_RCC_PWR_CLK_ENABLE();
	  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	  /** Initializes the RCC Oscillators according to the specified parameters
	  * in the RCC_OscInitTypeDef structure. */

	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	  RCC_OscInitStruct.PLL.PLLM = 8;
	  RCC_OscInitStruct.PLL.PLLN = 180;
	  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	  RCC_OscInitStruct.PLL.PLLQ = 2;
	  RCC_OscInitStruct.PLL.PLLR = 2;

	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
		  BGR_Error_Handler();
	  }

	  /** Activate the Over-Drive mode*/
	  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
	  {
		  BGR_Error_Handler();
	  }

	  /** Initializes the CPU, AHB and APB buses clocks */
	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	  {
		  BGR_Error_Handler();
	  }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 4.                                           | GPIO Initialization |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_GPIO_Init(void)
{
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC,&GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC,&GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC,&GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC,&GPIO_InitStruct);

	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 5.                                             | LED Management |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_LED_Manage_Output(uint8_t led_no)
{
	switch(led_no)
	{
	case 1 :

		HAL_GPIO_WritePin(LED1_PORT,LED1_PIN_NO,GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED2_PORT,LED2_PIN_NO,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED3_PORT,LED3_PIN_NO,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED4_PORT,LED4_PIN_NO,GPIO_PIN_RESET);
		break;

	case 2 :

		HAL_GPIO_WritePin(LED1_PORT,LED1_PIN_NO,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED2_PORT,LED2_PIN_NO,GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED3_PORT,LED3_PIN_NO,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED4_PORT,LED4_PIN_NO,GPIO_PIN_RESET);
		break;

	case 3 :

		HAL_GPIO_WritePin(LED1_PORT,LED1_PIN_NO,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED2_PORT,LED2_PIN_NO,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED3_PORT,LED3_PIN_NO,GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED4_PORT,LED4_PIN_NO,GPIO_PIN_RESET);
		break;

	case 4 :

		HAL_GPIO_WritePin(LED1_PORT,LED1_PIN_NO,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED2_PORT,LED2_PIN_NO,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED3_PORT,LED3_PIN_NO,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED4_PORT,LED4_PIN_NO,GPIO_PIN_SET);
		break;

	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 6.                                          | Basic Timer Initialization |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_TIMER6_Init(void)
{
	htimer6.Instance = TIM6;
	htimer6.Init.Prescaler = 1000;
	htimer6.Init.Period = 44970-1;

	if( HAL_TIM_Base_Init(&htimer6) != HAL_OK )
	{
		BGR_Error_Handler();
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 7.                                            | CAN - Initialization |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_CAN1_Init(void)
{
	hcan1.Instance = CAN1;
	hcan1.Init.Mode = CAN_MODE_NORMAL;
	hcan1.Init.AutoBusOff = ENABLE;
	hcan1.Init.AutoRetransmission = DISABLE;
	hcan1.Init.AutoWakeUp = ENABLE;
	hcan1.Init.ReceiveFifoLocked = DISABLE;
	hcan1.Init.TimeTriggeredMode = DISABLE;
	hcan1.Init.TransmitFifoPriority = DISABLE;

	//Settings related to CAN bit timings  - http://www.bittiming.can-wiki.info/
	hcan1.Init.Prescaler = 6;
	hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan1.Init.TimeSeg1 = CAN_BS1_12TQ;
	hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;

	if ( HAL_CAN_Init (&hcan1) != HAL_OK)
	{
		BGR_Error_Handler();
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 8.                                        | CAN - Filter Configurations |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_CAN_Filter_Config(void)
{
	can1_filter_init.FilterActivation = ENABLE;
	can1_filter_init.FilterFIFOAssignment = CAN_RX_FIFO0;
	can1_filter_init.FilterMode = CAN_FILTERMODE_IDMASK;
	can1_filter_init.FilterScale = CAN_FILTERSCALE_32BIT;
	can1_filter_init.SlaveStartFilterBank = 0;

/*	We need to mask only the first 11 MSB of register "FilterIdHigh" to filter the address:

	|		   FilterIdHigh (16 bit)			|				            FilterIdLow (16 bit)				   |
	| STID[10:0] (11 bit) | EXID[17:13] (5 bit) | EXID[12:0] (13 bit) |IDE[0] (1 bit) | RTR[0] (1 bit) | 0 (1 bit) |

	Note: even though FilterIdHigh/FilterIdLow are 32 bit, each one of them contains data of 16 bit.						*/

		can1_filter_init.FilterBank  = 0;					 // Filter number 0.
		can1_filter_init.FilterIdHigh = 0x0C00;				 // id_0x60 - 00001100000|00000
		can1_filter_init.FilterIdLow = 0x0000;
		can1_filter_init.FilterMaskIdHigh = 0XFFE0;          // mask    - 11111111111|00000
		can1_filter_init.FilterMaskIdLow = 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan1,&can1_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		can1_filter_init.FilterBank  = 1;					 // Filter number 1.
		can1_filter_init.FilterIdHigh = 0x0E00;				 // id_0x70 - 00001110000|00000
		can1_filter_init.FilterIdLow = 0x0000;
		can1_filter_init.FilterMaskIdHigh = 0XFFE0;          // mask    - 11111111111|00000
		can1_filter_init.FilterMaskIdLow = 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan1,&can1_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		can1_filter_init.FilterBank  = 2;					 // Filter number 2.
		can1_filter_init.FilterIdHigh = 0x1000;				 // id_0x80 - 00010000000|00000
		can1_filter_init.FilterIdLow = 0x0000;
		can1_filter_init.FilterMaskIdHigh = 0XFFE0;          // mask    - 11111111111|00000
		can1_filter_init.FilterMaskIdLow = 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan1,&can1_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		can1_filter_init.FilterBank  = 3;					 // Filter number 3.
		can1_filter_init.FilterIdHigh = 0x1200;				 // id_0x90 - 00010010000|00000
		can1_filter_init.FilterIdLow = 0x0000;
		can1_filter_init.FilterMaskIdHigh = 0XFFE0;          // mask    - 11111111111|00000
		can1_filter_init.FilterMaskIdLow = 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan1,&can1_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 9.                                       | CAN - Message Transmitting |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_CAN1_Tx(uint8_t node,uint8_t Our_Message[8])
{
	switch(node)
	{
		case 1:

			TxHeader.DLC = 8;
			TxHeader.StdId = 0x10;
			TxHeader.IDE   = CAN_ID_STD;
			TxHeader.RTR = CAN_RTR_DATA;
			break;

		case 2:

			TxHeader.DLC = 8;
			TxHeader.StdId = 0x20;
			TxHeader.IDE   = CAN_ID_STD;
			TxHeader.RTR = CAN_RTR_REMOTE;
			CanNode = 3;
			break;

		case 3:

			TxHeader.DLC = 8;
			TxHeader.StdId = 0x30;
			TxHeader.IDE   = CAN_ID_STD;
			TxHeader.RTR = CAN_RTR_REMOTE;
			CanNode = 4;
			break;

		case 4:

			TxHeader.DLC = 8;
			TxHeader.StdId = 0x40;
			TxHeader.IDE   = CAN_ID_STD;
			TxHeader.RTR = CAN_RTR_REMOTE;
			CanNode = 5;
			break;

		case 5:

			TxHeader.DLC = 8;
			TxHeader.StdId = 0x50;
			TxHeader.IDE   = CAN_ID_STD;
			TxHeader.RTR = CAN_RTR_REMOTE;
			CanNode = 2;
			break;
	}

	if( HAL_CAN_AddTxMessage(&hcan1,&TxHeader,Our_Message,&TxMailbox) != HAL_OK)
	{
		BGR_Error_Handler();
	}

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 10.                                        | CAN - Message Receiving |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_CAN1_Rx(void)
{
	if(HAL_CAN_GetRxMessage(&hcan1,CAN_RX_FIFO0,&RxHeader,rcvd_msg) != HAL_OK)
	{
		BGR_Error_Handler();
	}
	switch(RxHeader.StdId)
	{
	case 0x60:

		led_num = 1;
		BGR_LED_Manage_Output(led_num);
		BGR_CAN1_Tx(msg_check,rcvd_msg);
		break;

	case 0x70:

		led_num = 2;
		BGR_LED_Manage_Output(led_num);
		BGR_CAN1_Tx(msg_check,rcvd_msg);
		break;

	case 0x80:

		led_num = 3;
		BGR_LED_Manage_Output(led_num);
		BGR_CAN1_Tx(msg_check,rcvd_msg);
		break;

	case 0x90:

		led_num = 4;
		BGR_LED_Manage_Output(led_num);
		BGR_CAN1_Tx(msg_check,rcvd_msg);
		break;
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 11.                                             | Interrupt Management |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	uint8_t msg[8];

	BGR_CAN1_Tx(CanNode,msg);
	HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);

}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
	BGR_CAN1_Rx();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 12.                                                  | Error Handler |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_Error_Handler(void)
{
	while(1);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~| FIN |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
