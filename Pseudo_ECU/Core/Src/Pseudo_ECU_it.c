/*
 * it.c
 *
 *  Created on: 02-Jun-2018
 *      Author: kiran
 */

#include <Pseudo_ECU_main.h>

extern TIM_HandleTypeDef htimer6;
extern CAN_HandleTypeDef hcan1;
extern void Error_handler(void);

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}


void TIM6_DAC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htimer6);
}

void CAN1_TX_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hcan1);
}

void CAN1_RX0_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hcan1);
}


void CAN1_SCE_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hcan1);
}

void EXTI15_10_IRQHandler(void)
{
	CAN_TxHeaderTypeDef TxHeader;
	uint32_t TxMailbox;
	uint8_t Our_Message[8];

	TxHeader.DLC = 8;
	TxHeader.StdId = 0x20;
	TxHeader.IDE   = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_REMOTE;

	if( HAL_CAN_AddTxMessage(&hcan1,&TxHeader,Our_Message,&TxMailbox) != HAL_OK)
	{
		Error_handler();
	}

	HAL_TIM_Base_Start_IT(&htimer6);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);

}
