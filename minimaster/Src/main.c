/*

 * main.c
 *
 *  Created on: 02-Jun-2018
 *      Author: kiran
 */


#include "stm32f3xx_hal.h"
#include "main.h"

void SystemClockConfig(void);
void GPIO_Init(void);
void Error_handler(void);
void CAN_Init(void);
void CAN_Filter_Config(void);
void CAN_Tx(uint8_t node);
void CAN_Rx(void);
void LED_Manage_Output(uint8_t led_no);

TIM_HandleTypeDef htimer6;
CAN_HandleTypeDef hcan;
uint32_t Counter = 0;

//	SystemClock variables:
RCC_OscInitTypeDef RCC_OscInitStruct = {0};
RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

int main(void)
{
	HAL_Init();
	SystemClockConfig();
	GPIO_Init();
	CAN_Init();
	CAN_Filter_Config();

	if( HAL_CAN_Start(&hcan) != HAL_OK)
		 {
			Error_handler();
		 }

	 if(HAL_CAN_ActivateNotification(&hcan,CAN_IT_TX_MAILBOX_EMPTY|CAN_IT_RX_FIFO0_MSG_PENDING|CAN_IT_BUSOFF)!= HAL_OK)
	 	 {
		 	 Error_handler();
	 	 }

	while(1);

	return 0;
}


void SystemClockConfig(void)
{
	  /** Initializes the RCC Oscillators according to the specified parameters
	  * in the RCC_OscInitTypeDef structure.   */

	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;

	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
		  Error_handler();
	  }

	  /** Initializes the CPU, AHB and APB buses clocks*/

	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	  {
		  Error_handler();
	  }

	  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC12;
	  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;

	  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	  {
		  Error_handler();
	  }
}

void GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef ledgpio;

	ledgpio.Pin = GPIO_PIN_3;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB,&ledgpio);

	ledgpio.Pin = GPIO_PIN_5;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&ledgpio);

	ledgpio.Pin = GPIO_PIN_4;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&ledgpio);

	ledgpio.Pin = GPIO_PIN_6;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&ledgpio);

	ledgpio.Pin = GPIO_PIN_7;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&ledgpio);

}
void LED_Manage_Output(uint8_t led_no)
{

	switch(led_no)
	{
		case 1:
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
			break;

		case 2:
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
			break;

		case 3:
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
			break;

		case 4:
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
			break;
	}
}


void CAN_Tx(uint8_t node)
{

	CAN_TxHeaderTypeDef TxHeader;
	uint32_t TxMailbox;
	uint8_t var1 ,var2 ,var3 ,var4 ,var5 ,var6 ,var7 ,var8;

	switch(node)
	{
		case 1:
			// the content of the massage each var is 1 byte
			var1 = Counter + 1;
			var2 = Counter + 2;
			var3 = Counter + 3;
			var4 = Counter + 4;
			var5 = Counter + 5;
			var6 = Counter + 6;
			var7 = Counter + 7;
			var8 = Counter + 8;

			TxHeader.DLC = 8;
			TxHeader.StdId = 0x205;
			TxHeader.IDE   = CAN_ID_STD;
			TxHeader.RTR = CAN_RTR_DATA;
			break;

		case 2:
			// the content of the massage each var is 1 byte
			var1 = Counter + 1;
			var2 = Counter + 2;
			var3 = Counter + 3;
			var4 = Counter + 4;
			var5 = Counter + 5;
			var6 = Counter + 6;
			var7 = Counter + 7;
			var8 = Counter + 8;

			TxHeader.DLC = 8;
			TxHeader.StdId = 0x215;
			TxHeader.IDE   = CAN_ID_STD;
			TxHeader.RTR = CAN_RTR_DATA;
			break;

		case 3:
			// the content of the massage each var is 1 byte
			var1 = Counter + 1;
			var2 = Counter + 2;
			var3 = Counter + 3;
			var4 = Counter + 4;
			var5 = Counter + 5;
			var6 = Counter + 6;
			var7 = Counter + 7;
			var8 = Counter + 8;

			TxHeader.DLC = 8;
			TxHeader.StdId = 0x225;
			TxHeader.IDE   = CAN_ID_STD;
			TxHeader.RTR = CAN_RTR_DATA;
			break;

		case 4:
			// the content of the massage each var is 1 byte
			var1 = Counter + 1;
			var2 = Counter + 2;
			var3 = Counter + 3;
			var4 = Counter + 4;
			var5 = Counter + 5;
			var6 = Counter + 6;
			var7 = Counter + 7;
			var8 = Counter + 8;

			TxHeader.DLC = 8;
			TxHeader.StdId = 0x235;
			TxHeader.IDE   = CAN_ID_STD;
			TxHeader.RTR = CAN_RTR_DATA;
			break;
		case 5:
			// the content of the massage each var is 1 byte
			var1 = Counter + 1;
			var2 = Counter + 2;
			var3 = Counter + 3;
			var4 = Counter + 4;
			var5 = Counter + 5;
			var6 = Counter + 6;
			var7 = Counter + 7;
			var8 = Counter + 8;

			TxHeader.DLC = 8;
			TxHeader.StdId = 0x245;
			TxHeader.IDE   = CAN_ID_STD;
			TxHeader.RTR = CAN_RTR_DATA;
			break;

		case 6:
			// the content of the massage each var is 1 byte
			var1 = Counter + 1;
			var2 = Counter + 2;
			var3 = Counter + 3;
			var4 = Counter + 4;
			var5 = Counter + 5;
			var6 = Counter + 6;
			var7 = Counter + 7;
			var8 = Counter + 8;

			TxHeader.DLC = 8;
			TxHeader.StdId = 0x255;
			TxHeader.IDE   = CAN_ID_STD;
			TxHeader.RTR = CAN_RTR_DATA;
			break;

		case 7:
			// the content of the massage each var is 1 byte
			var1 = Counter + 1;
			var2 = Counter + 2;
			var3 = Counter + 3;
			var4 = Counter + 4;
			var5 = Counter + 5;
			var6 = Counter + 6;
			var7 = Counter + 7;
			var8 = Counter + 8;

			TxHeader.DLC = 8;
			TxHeader.StdId = 0x265;
			TxHeader.IDE   = CAN_ID_STD;
			TxHeader.RTR = CAN_RTR_DATA;
			break;

		case 8:
			// the content of the massage each var is 1 byte
			var1 = Counter + 1;
			var2 = Counter + 2;
			var3 = Counter + 3;
			var4 = Counter + 4;
			var5 = Counter + 5;
			var6 = Counter + 6;
			var7 = Counter + 7;
			var8 = Counter + 8;

			TxHeader.DLC = 8;
			TxHeader.StdId = 0x275;
			TxHeader.IDE   = CAN_ID_STD;
			TxHeader.RTR = CAN_RTR_DATA;
			break;

		case 9:
			// the content of the massage each var is 1 byte
			var1 = Counter + 1;
			var2 = Counter + 2;
			var3 = Counter + 3;
			var4 = Counter + 4;
			var5 = Counter + 5;
			var6 = Counter + 6;
			var7 = Counter + 7;
			var8 = Counter + 8;

			TxHeader.DLC = 8;
			TxHeader.StdId = 0x285;
			TxHeader.IDE   = CAN_ID_STD;
			TxHeader.RTR = CAN_RTR_DATA;
			break;

		case 10:
			// the content of the massage each var is 1 byte
			var1 = Counter + 1;
			var2 = Counter + 2;
			var3 = Counter + 3;
			var4 = Counter + 4;
			var5 = Counter + 5;
			var6 = Counter + 6;
			var7 = Counter + 7;
			var8 = Counter + 8;

			TxHeader.DLC = 8;
			TxHeader.StdId = 0x295;
			TxHeader.IDE   = CAN_ID_STD;
			TxHeader.RTR = CAN_RTR_DATA;
			break;
	}

	uint8_t Our_Message[8] = {var1,var2,var3,var4,var5,var6,var7,var8};

	if( HAL_CAN_AddTxMessage(&hcan,&TxHeader,Our_Message,&TxMailbox) != HAL_OK)
	{
		Error_handler();
	}
	Counter = Counter + 8;
}

void CAN_Rx(void)
{
	uint8_t rcvd_msg[8];
	CAN_RxHeaderTypeDef RxHeader;
	uint8_t CanNode;


	if(HAL_CAN_GetRxMessage(&hcan,CAN_RX_FIFO0,&RxHeader,rcvd_msg) != HAL_OK)
	{
		Error_handler();
	}

  	switch(RxHeader.StdId)
	{
	case 0x200:

		CanNode = 1;
		CAN_Tx(CanNode);
		break;

	case 0x210:

		CanNode = 2;
		CAN_Tx(CanNode);
		break;

	case 0x220:

		CanNode = 3;
		CAN_Tx(CanNode);
		break;

	case 0x230:

		CanNode = 4;
		CAN_Tx(CanNode);
		break;

	case 0x240:

		CanNode = 5;
		CAN_Tx(CanNode);
		break;

	case 0x250:

		CanNode = 6;
		CAN_Tx(CanNode);
		break;

	case 0x260:

		CanNode = 7;
		CAN_Tx(CanNode);
		break;

	case 0x270:

		CanNode = 8;
		CAN_Tx(CanNode);
		break;

	case 0x280:

		CanNode = 9;
		CAN_Tx(CanNode);
		break;

	case 0x290:

		CanNode = 10;
		CAN_Tx(CanNode);
		break;
	}
}
void CAN_Init(void)
{
	hcan.Instance = CAN;
	hcan.Init.Mode = CAN_MODE_NORMAL;
	hcan.Init.AutoBusOff = ENABLE;
	hcan.Init.AutoRetransmission = DISABLE;
	hcan.Init.AutoWakeUp = ENABLE;
	hcan.Init.ReceiveFifoLocked = DISABLE;
	hcan.Init.TimeTriggeredMode = DISABLE;
	hcan.Init.TransmitFifoPriority = DISABLE;

	//Settings related to CAN bit timings
	hcan.Init.Prescaler = 2;
	hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan.Init.TimeSeg1 = CAN_BS1_13TQ;
	hcan.Init.TimeSeg2 = CAN_BS2_2TQ;

	if ( HAL_CAN_Init (&hcan) != HAL_OK)
	{
		Error_handler();
	}
}

void CAN_Filter_Config(void)
{
	CAN_FilterTypeDef can_filter_init;

	can_filter_init.FilterActivation = ENABLE;
	can_filter_init.FilterFIFOAssignment = CAN_RX_FIFO0;
	can_filter_init.FilterMode = CAN_FILTERMODE_IDMASK;
	can_filter_init.FilterScale = CAN_FILTERSCALE_32BIT;
	can_filter_init.SlaveStartFilterBank = 0;


//		can_filter_init.FilterBank  = 0;
//		can_filter_init.FilterIdHigh = 0x0400;  // id_0x20
//		can_filter_init.FilterIdLow = 0x0000;
//		can_filter_init.FilterMaskIdHigh = 0XFFE0;
//		can_filter_init.FilterMaskIdLow = 0x0000;
//
//		if( HAL_CAN_ConfigFilter(&hcan,&can_filter_init) != HAL_OK)
//		{
//			Error_handler();
//		}
//
//		can_filter_init.FilterBank  = 1;
//		can_filter_init.FilterIdHigh = 0x0600;  // id_0x30
//		can_filter_init.FilterIdLow = 0x0000;
//		can_filter_init.FilterMaskIdHigh = 0XFFE0;
//		can_filter_init.FilterMaskIdLow = 0x0000;
//
//		if( HAL_CAN_ConfigFilter(&hcan,&can_filter_init) != HAL_OK)
//		{
//			Error_handler();
//		}
//
//		can_filter_init.FilterBank  = 2;
//		can_filter_init.FilterIdHigh = 0x0800;  // id_0x40
//		can_filter_init.FilterIdLow = 0x0000;
//		can_filter_init.FilterMaskIdHigh = 0XFFE0;
//		can_filter_init.FilterMaskIdLow = 0x0000;
//
//		if( HAL_CAN_ConfigFilter(&hcan,&can_filter_init) != HAL_OK)
//		{
//			Error_handler();
//		}
//
//		can_filter_init.FilterBank  = 3;
//		can_filter_init.FilterIdHigh = 0x0A00;  // id_0x50
//		can_filter_init.FilterIdLow = 0x0000;
//		can_filter_init.FilterMaskIdHigh = 0XFFF0;
//		can_filter_init.FilterMaskIdLow = 0x0000;
//
//		if( HAL_CAN_ConfigFilter(&hcan,&can_filter_init) != HAL_OK)
//		{
//			Error_handler();
//		}

		can_filter_init.FilterBank  = 4;
		can_filter_init.FilterIdHigh = 0x0000;  // accept all
		can_filter_init.FilterIdLow = 0x0000;
		can_filter_init.FilterMaskIdHigh = 0x0000;
		can_filter_init.FilterMaskIdLow = 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan,&can_filter_init) != HAL_OK)
		{
			Error_handler();
		}
}



void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_3);
	CAN_Rx();
}

void Error_handler(void)
{
	while(1);
}

