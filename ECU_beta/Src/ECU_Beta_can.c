#include <ECU_Beta_main.h>
#include <ECU_Beta_lwip.h>

void BGR_CAN1_Rx(void);

extern void BGR_Error_Handler(void);
extern void BGR_LED_Management(uint8_t LED,uint32_t Value);

//	CAN1 variables:
extern CAN_HandleTypeDef 		hcan1;
extern CAN_RxHeaderTypeDef 		RxHeader1;
extern CAN_FilterTypeDef 		can1_filter_init;
extern uint8_t 					Our_Last_Message[8];
extern uint8_t 					CanNode1;
extern uint8_t 					STATUS;
extern uint32_t                 Warning_flag;
extern uint32_t                 Maxvalue;
extern uint32_t                 MINvalue;
extern uint32_t                 Maxvalue2;
extern uint32_t                 MINvalue2;
//  B-Timer variables:
extern TIM_HandleTypeDef 		htimer7;

extern UART_HandleTypeDef 		huart3 ;
extern	char 					msg[100] ;


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
		can1_filter_init.FilterIdHigh 		= 0x0600;  // id_0x30
		can1_filter_init.FilterIdLow 		= 0x0000;
		can1_filter_init.FilterMaskIdHigh 	= 0XFFF0;
		can1_filter_init.FilterMaskIdLow 	= 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan1,&can1_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		can1_filter_init.FilterBank  		= 1;
		can1_filter_init.FilterIdHigh 		= 0x0800;  // id_0x40
		can1_filter_init.FilterIdLow 		= 0x0000;
		can1_filter_init.FilterMaskIdHigh 	= 0XFFF0;
		can1_filter_init.FilterMaskIdLow 	= 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan1,&can1_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		can1_filter_init.FilterBank  		= 2;
		can1_filter_init.FilterIdHigh 		= 0x0A00;  // id_0x50
		can1_filter_init.FilterIdLow 		= 0x0000;
		can1_filter_init.FilterMaskIdHigh 	= 0XFFF0;
		can1_filter_init.FilterMaskIdLow 	= 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan1,&can1_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}

		can1_filter_init.FilterBank  		= 3;
		can1_filter_init.FilterIdHigh 		= 0x24A0;  // id_0x125
		can1_filter_init.FilterIdLow 		= 0x0000;
		can1_filter_init.FilterMaskIdHigh 	= 0XFFF0;
		can1_filter_init.FilterMaskIdLow 	= 0x0000;

		if( HAL_CAN_ConfigFilter(&hcan1,&can1_filter_init) != HAL_OK)
		{
			BGR_Error_Handler();
		}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                         | CAN1 - Message Transmitting |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void BGR_CAN1_Tx(uint8_t node,uint8_t Our_Message[8])
{

	CAN_TxHeaderTypeDef TxHeader;
	uint32_t 			TxMailbox;

	switch(node)
	{
		case 1:

			TxHeader.DLC 	= 8;
			TxHeader.StdId 	= 0x110;
			TxHeader.IDE   	= CAN_ID_STD;
			TxHeader.RTR 	= CAN_RTR_DATA;
			break;

		case 2:

			TxHeader.DLC 	= 8;
			TxHeader.StdId 	= 0x120;
			TxHeader.IDE   	= CAN_ID_STD;
			TxHeader.RTR 	= CAN_RTR_REMOTE;
			break;

		case 3:

			TxHeader.DLC 	= 8;
			TxHeader.StdId 	= 0x31;
			TxHeader.IDE   	= CAN_ID_STD;
			TxHeader.RTR 	= CAN_RTR_REMOTE;
			break;

		case 4:

			TxHeader.DLC 	= 8;
			TxHeader.StdId 	= 0x41;
			TxHeader.IDE   	= CAN_ID_STD;
			TxHeader.RTR 	= CAN_RTR_REMOTE;
			break;

		case 5:

			TxHeader.DLC 	= 8;
			TxHeader.StdId 	= 0x51;
			TxHeader.IDE   	= CAN_ID_STD;
			TxHeader.RTR 	= CAN_RTR_REMOTE;
			break;
	}

	if( HAL_CAN_AddTxMessage(&hcan1,&TxHeader,Our_Message,&TxMailbox) != HAL_OK)
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
	uint8_t msg_check 		= {0};
	uint8_t FACTOR	 		= {0};
	uint32_t Accelerator_1  = {0};
	uint32_t Accelerator_2 	= {0};
	uint32_t Torque     	= {0};
	uint32_t Brakes			= {0};
	uint32_t Torque_check  	= {0};
	uint32_t difference		= {0};

	float temp;

	if(HAL_CAN_GetRxMessage(&hcan1,CAN_RX_FIFO0,&RxHeader1,rcvd_msg) != HAL_OK)
	{
		BGR_Error_Handler();
	}

	else
	{

//		sprintf(msg,"Massage received from CAN1 Id: 0x%x\r\n\n",RxHeader1.StdId);
//		HAL_UART_Transmit(&huart3,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);
	}

	switch(RxHeader1.StdId)
	{

		case 0x30:

			STATUS = NEUTRAL;
			msg_check = STATUS + 2;
			BGR_CAN1_Tx(msg_check,rcvd_msg);
			BGR_LED_Management(STATUS,0);
			break;

		case 0x40:

			if(STATUS == NEUTRAL)
			{
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);  //Buzzer
				HAL_Delay(1000);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);  //Buzzer
				HAL_Delay(200);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);
				HAL_Delay(100);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
				HAL_Delay(100);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);
				HAL_Delay(100);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
				STATUS = DRIVE;
			}

			msg_check = STATUS + 2;
			BGR_CAN1_Tx(msg_check,rcvd_msg);
			BGR_LED_Management(STATUS,0);
			break;

		case 0x50:

			if(STATUS == NEUTRAL)
			{
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);  //Buzzer
				HAL_Delay(1000);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);  //Buzzer
				HAL_Delay(200);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);
				HAL_Delay(100);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
				HAL_Delay(100);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);
				HAL_Delay(100);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
				STATUS = TRAINER;
			}

			msg_check = STATUS + 2;
			BGR_CAN1_Tx(msg_check,rcvd_msg);
			BGR_LED_Management(STATUS,0);
			break;

		case 0x125:

			msg_check = 1;
			BGR_CAN1_Tx(msg_check,rcvd_msg);

			// Saving our last message for global use
			Our_Last_Message[0] = rcvd_msg[0];
			Our_Last_Message[1] = rcvd_msg[1];
			Our_Last_Message[2] = rcvd_msg[2];
			Our_Last_Message[3] = rcvd_msg[3];
			Our_Last_Message[4] = rcvd_msg[4];
			Our_Last_Message[5] = rcvd_msg[5];
			Our_Last_Message[6] = rcvd_msg[6];
			Our_Last_Message[7] = rcvd_msg[7];

			switch(STATUS)
			{
				case NEUTRAL:

					FACTOR = 0;
					break;

				case DRIVE:

					FACTOR = 100;
					break;

				case TRAINER:

					FACTOR = 50;
					break;
			}

			Accelerator_1 = (rcvd_msg[0] << 8) + rcvd_msg[1];
			Accelerator_2 = (rcvd_msg[2] << 8) + rcvd_msg[3];
			Brakes = (rcvd_msg[4] << 8) + rcvd_msg[5];

			//	Accelerator_2 = 4096 - Accelerator_2;

			if (Accelerator_1 > Maxvalue)
			{
				Accelerator_1 = Maxvalue;

			}

			if (Accelerator_1 < MINvalue)
			{
				Accelerator_1 = MINvalue;

			}

			if (Accelerator_2 > Maxvalue2)
			{
				Accelerator_2 = Maxvalue2;

			}

			if (Accelerator_2 < MINvalue2)
			{
				Accelerator_2 = MINvalue2;

			}

			temp = (FACTOR*(Accelerator_1-MINvalue)/(Maxvalue-MINvalue));
			Torque = ((uint32_t)temp);

			temp = (FACTOR*(Brakes-MINvalue)/(Maxvalue-MINvalue));
			Brakes = ((uint32_t)temp);

			temp = (100*(Accelerator_2-MINvalue2)/(Maxvalue2-MINvalue2));
			Torque_check = ((uint32_t)temp);

			difference = Torque_check - Torque;

			sprintf(msg,"Torque: %d,Brakes: %d, Torque_check: %d, difference: %d, Accelerator_1: %d, rcvd_msg[0]: %d. rcvd_msg[1]: %d.  Accelerator_2: %d \r\n\n",Torque,Brakes,Torque_check,difference,Accelerator_1, rcvd_msg[0], rcvd_msg[1],Accelerator_2);
			HAL_UART_Transmit(&huart3,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

			sprintf(msg,"MAX Value: %d, MIN value: %d \r\n\n",Maxvalue,MINvalue);
			HAL_UART_Transmit(&huart3,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

			if(Torque > 30 && Torque_check > 30)
			{
				Warning_flag = 1;
				Torque = 0;
				//HAL_TIM_Base_Start_IT(&htimer7);
				BGR_LED_Management(WARNING,1);

			}

			else
			{
				Warning_flag = 0;
				//HAL_TIM_Base_Stop_IT(&htimer7);
				BGR_LED_Management(WARNING,2);
			}

			BGR_LED_Management(Torques_LED,Torque);
			BGR_LED_Management(Brakes_LED,Torque_check);

			if(Start_Engines() != ERR_OK)
			{
				BGR_Error_Handler();
			}


			if(Go_Baby_Go(Torque) != ERR_OK)
			{
				BGR_Error_Handler();
			}

			break;
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
//  HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_14);
}

