#include <Data_Logger_main.h>
#include <Data_Logger_lwip.h>

void BGR_CAN1_Rx(void);

extern void BGR_CAN1_Tx(uint8_t node);
extern void BGR_CAN2_Tx(uint8_t node,uint8_t Our_Message2[8]);
extern void BGR_LED_Management(uint8_t LED);
extern void BGR_Error_Handler(void);

extern TIM_HandleTypeDef 	htimer6;

//	CAN1 variables:
extern CAN_HandleTypeDef 	hcan1;
extern CAN_RxHeaderTypeDef 	RxHeader1;
extern CAN_FilterTypeDef 	can1_filter_init;
extern CAN_FilterTypeDef 	can2_filter_init;
extern uint8_t 				STATUS;


//	CAN2 variables:
extern CAN_HandleTypeDef 	hcan2;
extern CAN_RxHeaderTypeDef 	RxHeader2;
extern 	uint8_t 			CanNode2;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                             | CAN1 - Initialization |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_CAN1_Init(void)
{
	hcan1.Instance 						= CAN1;
	hcan1.Init.Mode 					= CAN_MODE_NORMAL;
	hcan1.Init.AutoBusOff 				= ENABLE;
	hcan1.Init.AutoRetransmission 		= DISABLE;
	hcan1.Init.AutoWakeUp 				= ENABLE;
	hcan1.Init.ReceiveFifoLocked 		= DISABLE;
	hcan1.Init.TimeTriggeredMode 		= DISABLE;
	hcan1.Init.TransmitFifoPriority 	= DISABLE;

	//Settings related to CAN bit timings
	hcan1.Init.Prescaler 				= 3;
	hcan1.Init.SyncJumpWidth 			= CAN_SJW_1TQ;
	hcan1.Init.TimeSeg1 				= CAN_BS1_15TQ;
	hcan1.Init.TimeSeg2 				= CAN_BS2_2TQ;

	if ( HAL_CAN_Init (&hcan1) != HAL_OK)
	{
		BGR_Error_Handler();
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                             | CAN2 - Initialization |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_CAN2_Init(void)
{
	hcan2.Instance 						= CAN2;
	hcan2.Init.Mode 					= CAN_MODE_NORMAL;
	hcan2.Init.AutoBusOff 				= ENABLE;
	hcan2.Init.AutoRetransmission 		= DISABLE;
	hcan2.Init.AutoWakeUp 				= ENABLE;
	hcan2.Init.ReceiveFifoLocked 		= DISABLE;
	hcan2.Init.TimeTriggeredMode 		= DISABLE;
	hcan2.Init.TransmitFifoPriority 	= DISABLE;

	//Settings related to CAN bit timings
	hcan2.Init.Prescaler 				= 3;
	hcan2.Init.SyncJumpWidth 			= CAN_SJW_1TQ;
	hcan2.Init.TimeSeg1 				= CAN_BS1_15TQ;
	hcan2.Init.TimeSeg2 				= CAN_BS2_2TQ;

	if (HAL_CAN_Init(&hcan2) != HAL_OK)
	{
	  BGR_Error_Handler();
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                         | CAN - Filter Configurations |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_CAN_Filter_Config(void)
{

/*	We need to mask only the first 11 MSB of register "FilterIdHigh" to filter the address's number:
	|		   FilterIdHigh (16 bit)			|				            FilterIdLow (16 bit)				   |
	| STID[10:0] (11 bit) | EXID[17:13] (5 bit) | EXID[12:0] (13 bit) |IDE[0] (1 bit) | RTR[0] (1 bit) | 0 (1 bit) |

	Note: even though FilterIdHigh/FilterIdLow are 32 bit, each one of them contains data of 16 bit.						*/

//CAN1

	can1_filter_init.FilterActivation 		= ENABLE;
	can1_filter_init.FilterFIFOAssignment 	= CAN_RX_FIFO0;
	can1_filter_init.FilterMode 			= CAN_FILTERMODE_IDMASK;
	can1_filter_init.FilterScale 			= CAN_FILTERSCALE_32BIT;
	can1_filter_init.SlaveStartFilterBank 	= 0;


		can1_filter_init.FilterBank  		= 0;
		can1_filter_init.FilterIdHigh 		= 0x0400;  // id_0x20
		can1_filter_init.FilterIdLow 		= 0x0000;
		can1_filter_init.FilterMaskIdHigh 	= 0XFFF0;
		can1_filter_init.FilterMaskIdLow 	= 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan1,&can1_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		can1_filter_init.FilterBank  		= 1;
		can1_filter_init.FilterIdHigh 		= 0x0420;  // id_0x21
		can1_filter_init.FilterIdLow 		= 0x0000;
		can1_filter_init.FilterMaskIdHigh 	= 0XFFF0;
		can1_filter_init.FilterMaskIdLow 	= 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan1,&can1_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		can1_filter_init.FilterBank  		= 2;
		can1_filter_init.FilterIdHigh 		= 0x0620;  // id_0x31
		can1_filter_init.FilterIdLow 		= 0x0000;
		can1_filter_init.FilterMaskIdHigh 	= 0XFFF0;
		can1_filter_init.FilterMaskIdLow 	= 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan1,&can1_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		can1_filter_init.FilterBank  		= 3;
		can1_filter_init.FilterIdHigh 		= 0x0820;  // id_0x41
		can1_filter_init.FilterIdLow 		= 0x0000;
		can1_filter_init.FilterMaskIdHigh 	= 0XFFF0;
		can1_filter_init.FilterMaskIdLow 	= 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan1,&can1_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		can1_filter_init.FilterBank  		= 4;
		can1_filter_init.FilterIdHigh 		= 0x0A20;  // id_0x51
		can1_filter_init.FilterIdLow 		= 0x0000;
		can1_filter_init.FilterMaskIdHigh 	= 0XFFF0;
		can1_filter_init.FilterMaskIdLow 	= 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan1,&can1_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		can1_filter_init.FilterBank  		= 5;
		can1_filter_init.FilterIdHigh 		= 0x24A0;  // id_0x125
		can1_filter_init.FilterIdLow 		= 0x0000;
		can1_filter_init.FilterMaskIdHigh 	= 0XFFF0;
		can1_filter_init.FilterMaskIdLow 	= 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan1,&can1_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}

//CAN2

	can2_filter_init.FilterActivation 		= ENABLE;
	can2_filter_init.FilterFIFOAssignment 	= CAN_RX_FIFO1;
	can2_filter_init.FilterMode 			= CAN_FILTERMODE_IDMASK;
	can2_filter_init.FilterScale 			= CAN_FILTERSCALE_32BIT;
	can2_filter_init.SlaveStartFilterBank 	= 14;


		can2_filter_init.FilterBank  		= 14;
		can2_filter_init.FilterIdHigh 		= 0x40A0;  //id_0x205
		can2_filter_init.FilterIdLow 		= 0x0000;
		can2_filter_init.FilterMaskIdHigh 	= 0XFFE0;
		can2_filter_init.FilterMaskIdLow 	= 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan2,&can2_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		can2_filter_init.FilterBank  		= 15;
		can2_filter_init.FilterIdHigh 		= 0x42A0;  //id_0x215
		can2_filter_init.FilterIdLow 		= 0x0000;
		can2_filter_init.FilterMaskIdHigh 	= 0XFFE0;
		can2_filter_init.FilterMaskIdLow 	= 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan2,&can2_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		can2_filter_init.FilterBank  		= 16;
		can2_filter_init.FilterIdHigh 		= 0x44A0;  //id_0x225
		can2_filter_init.FilterIdLow 		= 0x0000;
		can2_filter_init.FilterMaskIdHigh 	= 0XFFE0;
		can2_filter_init.FilterMaskIdLow 	= 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan2,&can2_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		can2_filter_init.FilterBank  		= 17;
		can2_filter_init.FilterIdHigh 		= 0x46A0;  //id_0x235
		can2_filter_init.FilterIdLow 		= 0x0000;
		can2_filter_init.FilterMaskIdHigh 	= 0XFFE0;
		can2_filter_init.FilterMaskIdLow 	= 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan2,&can2_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		can2_filter_init.FilterBank  		= 18;
		can2_filter_init.FilterIdHigh 		= 0x48A0;  //id_0x245
		can2_filter_init.FilterIdLow 		= 0x0000;
		can2_filter_init.FilterMaskIdHigh 	= 0XFFE0;
		can2_filter_init.FilterMaskIdLow 	= 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan2,&can2_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		can2_filter_init.FilterBank  		= 19;
		can2_filter_init.FilterIdHigh 		= 0x4AA0;  //id_0x255
		can2_filter_init.FilterIdLow 		= 0x0000;
		can2_filter_init.FilterMaskIdHigh 	= 0XFFE0;
		can2_filter_init.FilterMaskIdLow 	= 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan2,&can2_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		can2_filter_init.FilterBank  		= 20;
		can2_filter_init.FilterIdHigh 		= 0x4CA0;  //id_0x265
		can2_filter_init.FilterIdLow 		= 0x0000;
		can2_filter_init.FilterMaskIdHigh 	= 0XFFE0;
		can2_filter_init.FilterMaskIdLow 	= 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan2,&can2_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		can2_filter_init.FilterBank  		= 21;
		can2_filter_init.FilterIdHigh 		= 0x4EA0;  //id_0x275
		can2_filter_init.FilterIdLow 		= 0x0000;
		can2_filter_init.FilterMaskIdHigh 	= 0XFFE0;
		can2_filter_init.FilterMaskIdLow 	= 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan2,&can2_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		can2_filter_init.FilterBank  		= 22;
		can2_filter_init.FilterIdHigh 		= 0x50A0;  //id_0x285
		can2_filter_init.FilterIdLow 		= 0x0000;
		can2_filter_init.FilterMaskIdHigh 	= 0XFFE0;
		can2_filter_init.FilterMaskIdLow 	= 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan2,&can2_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		can2_filter_init.FilterBank  		= 23;
		can2_filter_init.FilterIdHigh 		= 0x52A0;  //id_0x295
		can2_filter_init.FilterIdLow 		= 0x0000;
		can2_filter_init.FilterMaskIdHigh 	= 0XFFE0;
		can2_filter_init.FilterMaskIdLow 	= 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan2,&can2_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                         | CAN1 - Message Transmitting |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_CAN1_Tx(uint8_t node)
{
	CAN_TxHeaderTypeDef 	TxHeader1;
	uint32_t 				TxMailbox1;
	uint8_t 				Our_Message1[8] = {0};

	switch(node)
	{
		case NEUTRAL:

			TxHeader1.DLC 	= 8;
			TxHeader1.StdId = 0x30;
			TxHeader1.IDE   = CAN_ID_STD;
			TxHeader1.RTR 	= CAN_RTR_REMOTE;
			break;

		case DRIVE:

			TxHeader1.DLC 	= 8;
			TxHeader1.StdId = 0x40;
			TxHeader1.IDE   = CAN_ID_STD;
			TxHeader1.RTR 	= CAN_RTR_REMOTE;
			break;

		case TRAINER:

			TxHeader1.DLC 	= 8;
			TxHeader1.StdId = 0x50;
			TxHeader1.IDE   = CAN_ID_STD;
			TxHeader1.RTR 	= CAN_RTR_REMOTE;
			break;

	}

	if( HAL_CAN_AddTxMessage(&hcan1,&TxHeader1,Our_Message1,&TxMailbox1) != HAL_OK)
	{
		BGR_Error_Handler();
	}

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                         | CAN1 - Message Receiving |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_CAN1_Rx(void)
{
	uint8_t rcvd_msg[8] 	= {0};
	uint8_t Id_Number		= {0};
	uint8_t msg_check 		= 1;
	uint8_t CAN_Number      = 1;
	uint8_t Flag_Number 	= 14;

	if(HAL_CAN_GetRxMessage(&hcan1,CAN_RX_FIFO0,&RxHeader1,rcvd_msg) != HAL_OK)
	{
		BGR_Error_Handler();
	}

//	else
//	{
//		sprintf(msg,"Massage received from CAN1 Id: 0x%x\r\n\n",RxHeader1.StdId);
//		HAL_UART_Transmit(&huart3,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);
//	}
	switch(RxHeader1.StdId)
	{
		case 0x20:

			HAL_Delay(2);
			HAL_TIM_Base_Start_IT(&htimer6);
			return;

		case 0x21:

			HAL_TIM_Base_Stop_IT(&htimer6);
			STATUS = NEUTRAL;
			BGR_LED_Management(STATUS);
			return;

		case 0x31:

			STATUS = NEUTRAL;
			BGR_LED_Management(STATUS);
			return;

		case 0x41:

			STATUS = DRIVE;
			BGR_LED_Management(STATUS);
			return;

		case 0x51:

			STATUS = TRAINER;
			BGR_LED_Management(STATUS);
			return;

		case 0x125:

			BGR_CAN2_Tx(msg_check,rcvd_msg);
			Id_Number 	= 0x25;
			Flag_Number = 13;
			break;
	}
	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);
  	if(Go_Baby_Go(Flag_Number,CAN_Number,Id_Number, rcvd_msg) != ERR_OK)
	{
		BGR_Error_Handler();
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                         | CAN2 - Message Transmitting |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_CAN2_Tx(uint8_t node,uint8_t Our_Message2[8])
{
	CAN_TxHeaderTypeDef 	TxHeader2;
	uint32_t 				TxMailbox2;

	switch(node)
	{
		case 1:

			TxHeader2.DLC 	= 8;
			TxHeader2.StdId = 0x11;
			TxHeader2.IDE   = CAN_ID_STD;
			TxHeader2.RTR 	= CAN_RTR_DATA;
			break;

		case 2:

			TxHeader2.DLC 	= 8;
			TxHeader2.StdId = 0x22;
			TxHeader2.IDE   = CAN_ID_STD;
			TxHeader2.RTR 	= CAN_RTR_DATA;
			break;

		case 3:

			TxHeader2.DLC 	= 8;
			TxHeader2.StdId = 0x200;
			TxHeader2.IDE   = CAN_ID_STD;
			TxHeader2.RTR 	= CAN_RTR_REMOTE;
			CanNode2 		= 4;
			break;

		case 4:

			TxHeader2.DLC 	= 8;
			TxHeader2.StdId = 0x210;
			TxHeader2.IDE   = CAN_ID_STD;
			TxHeader2.RTR 	= CAN_RTR_REMOTE;
			CanNode2 		= 5;
			break;

		case 5:

			TxHeader2.DLC 	= 8;
			TxHeader2.StdId = 0x220;
			TxHeader2.IDE   = CAN_ID_STD;
			TxHeader2.RTR 	= CAN_RTR_REMOTE;
			CanNode2 		= 6;
			break;

		case 6:

			TxHeader2.DLC 	= 8;
			TxHeader2.StdId = 0x230;
			TxHeader2.IDE   = CAN_ID_STD;
			TxHeader2.RTR 	= CAN_RTR_REMOTE;
			CanNode2 		= 7;
			break;

		case 7:

			TxHeader2.DLC 	= 8;
			TxHeader2.StdId = 0x240;
			TxHeader2.IDE   = CAN_ID_STD;
			TxHeader2.RTR 	= CAN_RTR_REMOTE;
			CanNode2 		= 8;
			break;

		case 8:

			TxHeader2.DLC 	= 8;
			TxHeader2.StdId = 0x250;
			TxHeader2.IDE   = CAN_ID_STD;
			TxHeader2.RTR 	= CAN_RTR_REMOTE;
			CanNode2 		= 9;
			break;

		case 9:

			TxHeader2.DLC 	= 8;
			TxHeader2.StdId = 0x260;
			TxHeader2.IDE   = CAN_ID_STD;
			TxHeader2.RTR 	= CAN_RTR_REMOTE;
			CanNode2 		= 10;
			break;

		case 10:

			TxHeader2.DLC 	= 8;
			TxHeader2.StdId = 0x270;
			TxHeader2.IDE   = CAN_ID_STD;
			TxHeader2.RTR 	= CAN_RTR_REMOTE;
			CanNode2 		= 11;
			break;

		case 11:

			TxHeader2.DLC 	= 8;
			TxHeader2.StdId = 0x280;
			TxHeader2.IDE   = CAN_ID_STD;
			TxHeader2.RTR 	= CAN_RTR_REMOTE;
			CanNode2 		= 12;
			break;

		case 12:

			TxHeader2.DLC 	= 8;
			TxHeader2.StdId = 0x290;
			TxHeader2.IDE   = CAN_ID_STD;
			TxHeader2.RTR 	= CAN_RTR_REMOTE;
			CanNode2 		= 3;
			break;
	}

	if( HAL_CAN_AddTxMessage(&hcan2,&TxHeader2,Our_Message2,&TxMailbox2) != HAL_OK)
	{
		BGR_Error_Handler();
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                         | CAN2 - Message Receiving |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_CAN2_Rx(void)
{
	uint8_t rcvd_msg[8]	= {0};
	uint8_t Id_Number 	= {0};
	uint8_t Flag_Number = 14;
	uint8_t CAN_Number 	= 2;
	uint8_t msg_check 	= 2;

	if(HAL_CAN_GetRxMessage(&hcan2,CAN_RX_FIFO1,&RxHeader2,rcvd_msg) != HAL_OK)
	{
		BGR_Error_Handler();
	}

//  	sprintf(msg,"Massage received from CAN2 Id: 0x%x : \r\n\n", RxHeader2.StdId);
//  	HAL_UART_Transmit(&huart3,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

	switch(RxHeader2.StdId)
	{
		case 0x205:

			BGR_CAN2_Tx(msg_check,rcvd_msg);
			Id_Number 	= 0x05;
			Flag_Number = 3;
			break;

		case 0x215:

			BGR_CAN2_Tx(msg_check,rcvd_msg);
			Id_Number 	= 0x15;
			Flag_Number = 4;
			break;

		case 0x225:

			BGR_CAN2_Tx(msg_check,rcvd_msg);
			Id_Number 	= 0x25;
			Flag_Number = 5;
			break;

		case 0x235:

			BGR_CAN2_Tx(msg_check,rcvd_msg);
			Id_Number 	= 0x35;
			Flag_Number = 6;
			break;

		case 0x245:

			BGR_CAN2_Tx(msg_check,rcvd_msg);
			Id_Number 	= 0x45;
			Flag_Number = 7;
			break;

		case 0x255:

			BGR_CAN2_Tx(msg_check,rcvd_msg);
			Id_Number 	= 0x55;
			Flag_Number = 8;
			break;

		case 0x265:

			BGR_CAN2_Tx(msg_check,rcvd_msg);
			Id_Number 	= 0x65;
			Flag_Number = 9;
			break;

		case 0x275:

			BGR_CAN2_Tx(msg_check,rcvd_msg);
			Id_Number 	= 0x75;
			Flag_Number = 10;
			break;

		case 0x285:

			BGR_CAN2_Tx(msg_check,rcvd_msg);
			Id_Number 	= 0x85;
			Flag_Number = 11;
			break;

		case 0x295:

			BGR_CAN2_Tx(msg_check,rcvd_msg);
			Id_Number 	= 0x95;
			Flag_Number = 12;
			break;
	}

	if(Go_Baby_Go(Flag_Number,CAN_Number, Id_Number, rcvd_msg) != ERR_OK)
	{
		BGR_Error_Handler();
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                              | CAN -Interrupt Management |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	BGR_CAN1_Rx();
//	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
//	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);
//	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_14);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	BGR_CAN2_Rx();
//	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
//	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);
//	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_14);
}
