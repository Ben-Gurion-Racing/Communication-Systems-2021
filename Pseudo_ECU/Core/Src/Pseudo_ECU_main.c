/*
 *                                            ~| Pseudo ECU - Project |~
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Name: Johnathan Dekel

 * main.c
 */

#include <Pseudo_ECU_main.h>
#include "stm32f4xx_hal.h"

void SystemClock_Config_HSE(uint8_t clock_freq);
void TIMER6_Init(void);
void GPIO_Init(void);
void Error_handler(void);
void CAN1_Init(void);
void CAN_Filter_Config(void);
void CAN1_Tx(uint8_t node,uint8_t Our_Message[8]);
void CAN1_Rx(void);
void LED_Manage_Output(uint8_t led_no);

TIM_HandleTypeDef 	htimer6;
CAN_HandleTypeDef 	hcan1;
uint8_t 		  	CanNode = 2;
CAN_RxHeaderTypeDef RxHeader;
CAN_TxHeaderTypeDef TxHeader;
uint32_t TxMailbox;
uint8_t Our_Message[8];

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                                      | Main |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main(void)
{
	HAL_Init();
	SystemClock_Config_HSE(SYS_CLOCK_FREQ_180_MHZ);
	GPIO_Init();
	TIMER6_Init();
	CAN1_Init();
	CAN_Filter_Config();

	 if( HAL_CAN_Start(&hcan1) != HAL_OK)
		{
			Error_handler();
		}

	 if(HAL_CAN_ActivateNotification(&hcan1,CAN_IT_TX_MAILBOX_EMPTY|CAN_IT_RX_FIFO0_MSG_PENDING|CAN_IT_BUSOFF)!= HAL_OK)
	 	{
	 		Error_handler();
	 	}

		TxHeader.DLC = 8;
		TxHeader.StdId = 0x21;
		TxHeader.IDE   = CAN_ID_STD;
		TxHeader.RTR = CAN_RTR_REMOTE;

		if( HAL_CAN_AddTxMessage(&hcan1,&TxHeader,Our_Message,&TxMailbox) != HAL_OK)
		{
			Error_handler();
		}

	while(1);

	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                         | System Clock Configurations |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void SystemClock_Config_HSE(uint8_t clock_freq)
{
	RCC_OscInitTypeDef osc_init = {0};
	RCC_ClkInitTypeDef clk_init = {0};
	uint32_t Flatency = 0;

		osc_init.OscillatorType			= RCC_OSCILLATORTYPE_HSE;
		osc_init.HSEState				= RCC_HSE_BYPASS; //FOR STM32F446RE MCU, HSE IS FROM ST-LINK CIRCUITRY... THEREFORE BYPASS
		//NO CALIBRATION REQUIRED FOR HSE
		osc_init.PLL.PLLState			= RCC_PLL_ON;
		osc_init.PLL.PLLSource			= RCC_PLLSOURCE_HSE;

		switch(clock_freq)
		{

		case SYS_CLOCK_FREQ_50_MHZ:
		{
			osc_init.PLL.PLLM 		= 8;
			osc_init.PLL.PLLN		= 100;
			osc_init.PLL.PLLP		= 2;
			osc_init.PLL.PLLR		= 2;
			osc_init.PLL.PLLQ		= 2;

			clk_init.ClockType 				= RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource			= RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider			= RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider			= RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider			= RCC_HCLK_DIV2;

			Flatency = FLASH_ACR_LATENCY_1WS;
			break;
		}

		case SYS_CLOCK_FREQ_84_MHZ:
		{
			osc_init.PLL.PLLM 		= 8;
			osc_init.PLL.PLLN		= 168;
			osc_init.PLL.PLLP		= 2;
			osc_init.PLL.PLLR		= 2;
			osc_init.PLL.PLLQ		= 2;

			clk_init.ClockType 				= RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource			= RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider			= RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider			= RCC_HCLK_DIV4;
			clk_init.APB2CLKDivider			= RCC_HCLK_DIV2;

			Flatency = FLASH_ACR_LATENCY_2WS;
			break;
		}

		case SYS_CLOCK_FREQ_120_MHZ:
		{
			osc_init.PLL.PLLM 		= 8;
			osc_init.PLL.PLLN		= 240;
			osc_init.PLL.PLLP		= 2;
			osc_init.PLL.PLLR		= 2;
			osc_init.PLL.PLLQ		= 2;

			clk_init.ClockType 				= RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource			= RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider			= RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider			= RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider			= RCC_HCLK_DIV2;

			Flatency = FLASH_ACR_LATENCY_3WS;
			break;
		}

		case SYS_CLOCK_FREQ_180_MHZ:
		{
			/*
			 * For 180mhz, you have to enable voltage scaling and also put the micro-controller in over-drive mode
			 * from Power control register---------------> PWR_CR
			 *
			 */

			//Enable the clk for pwer controller
			__HAL_RCC_PWR_CLK_ENABLE();

			//Set regulator voltage scale as 1
			__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

			//Turn on the overdrive mode
			__HAL_PWR_OVERDRIVE_ENABLE();

			osc_init.PLL.PLLM 		= 8;
			osc_init.PLL.PLLN		= 180;
			osc_init.PLL.PLLP		= 2;
			osc_init.PLL.PLLR		= 2;
			osc_init.PLL.PLLQ		= 2;

			clk_init.ClockType 				= RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource			= RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider			= RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider			= RCC_HCLK_DIV1;
			clk_init.APB2CLKDivider			= RCC_HCLK_DIV2;

			Flatency = FLASH_ACR_LATENCY_5WS;
			break;

		}

		default:
		return;

		}

	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_handler();
	}

	if(HAL_RCC_ClockConfig(&clk_init, Flatency) != HAL_OK)
	{
		Error_handler();
	}

	//SYSTICK CONFIGURATION
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                            | GPIO Initialization |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void GPIO_Init(void)
{
	   	__HAL_RCC_GPIOA_CLK_ENABLE();
	    __HAL_RCC_GPIOC_CLK_ENABLE();
	    __HAL_RCC_GPIOB_CLK_ENABLE();

		GPIO_InitTypeDef ledgpio;

		ledgpio.Pin = GPIO_PIN_5;
		ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
		ledgpio.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA,&ledgpio);


		ledgpio.Pin = GPIO_PIN_9;
		ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
		ledgpio.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOC,&ledgpio);


		ledgpio.Pin = GPIO_PIN_8;
		ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
		ledgpio.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOC,&ledgpio);


		ledgpio.Pin = GPIO_PIN_6;
		ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
		ledgpio.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOC,&ledgpio);


		ledgpio.Pin = GPIO_PIN_8;
		ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
		ledgpio.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB,&ledgpio);

		ledgpio.Pin = GPIO_PIN_13;
		ledgpio.Mode = GPIO_MODE_IT_FALLING;
		ledgpio.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOC,&ledgpio);

		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                              | LED Management |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void LED_Manage_Output(uint8_t led_no)
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
//                                           | Basic Timer Initialization |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void TIMER6_Init(void)
{
	htimer6.Instance = TIM6;
	htimer6.Init.Prescaler = 10;
	htimer6.Init.Period = 40908-1;

	if( HAL_TIM_Base_Init(&htimer6) != HAL_OK )
	{
		Error_handler();
	}

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                             | CAN - Initialization |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void CAN1_Init(void)
{
	hcan1.Instance = CAN1;
	hcan1.Init.Mode = CAN_MODE_NORMAL;
	hcan1.Init.AutoBusOff = ENABLE;
	hcan1.Init.AutoRetransmission = DISABLE;
	hcan1.Init.AutoWakeUp = ENABLE;
	hcan1.Init.ReceiveFifoLocked = DISABLE;
	hcan1.Init.TimeTriggeredMode = DISABLE;
	hcan1.Init.TransmitFifoPriority = DISABLE;

	//Settings related to CAN bit timings
	hcan1.Init.Prescaler = 5;
	hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan1.Init.TimeSeg1 = CAN_BS1_15TQ;
	hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;

	if ( HAL_CAN_Init (&hcan1) != HAL_OK)
	{
		Error_handler();
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                         | CAN - Filter Configurations |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void CAN_Filter_Config(void)
{
	CAN_FilterTypeDef can1_filter_init;

	can1_filter_init.FilterActivation = ENABLE;
	can1_filter_init.FilterFIFOAssignment = CAN_RX_FIFO0;
	can1_filter_init.FilterMode = CAN_FILTERMODE_IDMASK;
	can1_filter_init.FilterScale = CAN_FILTERSCALE_32BIT;


	can1_filter_init.FilterBank  = 0;
	can1_filter_init.FilterIdHigh = 0x24A0;   //id_0x125
	can1_filter_init.FilterIdLow = 0x0000;
	can1_filter_init.FilterMaskIdHigh = 0XFFE0;
	can1_filter_init.FilterMaskIdLow = 0x0000;

	if( HAL_CAN_ConfigFilter(&hcan1,&can1_filter_init) != HAL_OK)
	{
		Error_handler();
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                         | CAN - Message Transmitting |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void CAN1_Tx(uint8_t node,uint8_t Our_Message[8])
{

	CAN_TxHeaderTypeDef TxHeader;
	uint32_t TxMailbox;

	switch(node)
	{
		case 1:

			TxHeader.DLC = 8;
			TxHeader.StdId = 0x110;
			TxHeader.IDE   = CAN_ID_STD;
			TxHeader.RTR = CAN_RTR_DATA;
			break;

		case 2:

			TxHeader.DLC = 8;
			TxHeader.StdId = 0x120;
			TxHeader.IDE   = CAN_ID_STD;
			TxHeader.RTR = CAN_RTR_REMOTE;
			CanNode = 3;
			break;

		case 3:

			TxHeader.DLC = 8;
			TxHeader.StdId = 0x120;
			TxHeader.IDE   = CAN_ID_STD;
			TxHeader.RTR = CAN_RTR_REMOTE;
			CanNode = 4;
			break;

		case 4:

			TxHeader.DLC = 8;
			TxHeader.StdId = 0x120;
			TxHeader.IDE   = CAN_ID_STD;
			TxHeader.RTR = CAN_RTR_REMOTE;
			CanNode = 5;
			break;

		case 5:

			TxHeader.DLC = 8;
			TxHeader.StdId = 0x120;
			TxHeader.IDE   = CAN_ID_STD;
			TxHeader.RTR = CAN_RTR_REMOTE;
			CanNode = 2;
			break;
	}

	if( HAL_CAN_AddTxMessage(&hcan1,&TxHeader,Our_Message,&TxMailbox) != HAL_OK)
	{
		Error_handler();
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                         | CAN - Message Receiving |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void CAN1_Rx(void)
{
	uint8_t rcvd_msg[8];
	uint8_t msg_check = 1;

	if(HAL_CAN_GetRxMessage(&hcan1,CAN_RX_FIFO0,&RxHeader,rcvd_msg) != HAL_OK)
	{
		Error_handler();
	}
	switch(RxHeader.StdId)
	{
		case 0x125:

			CAN1_Tx(msg_check,rcvd_msg);
			break;
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                              | Interrupt Management |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	uint8_t msg[8];

	CAN1_Tx(CanNode,msg);
	HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);

}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	//HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
	CAN1_Rx();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                                  | Error Handler |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void Error_handler(void)
{
	while(1);

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~| FIN |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
