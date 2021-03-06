/*
 * msp.c
 *
 *  Created on: 02-Jun-2018
 *      Author: kiran
 */

#include "main.h"

void HAL_MspInit(void)
{
 //Here will do low level processor specific inits.
	//1. Set up the priority grouping of the arm cortex mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//2. Enable the required system exceptions of the arm cortex mx processor
	SCB->SHCSR |= 0x7 << 16; //usage fault, memory fault and bus fault system exceptions

	//3. configure the priority for the system exceptions
	HAL_NVIC_SetPriority(MemoryManagement_IRQn,0,0);
	HAL_NVIC_SetPriority(BusFault_IRQn,0,0);
	HAL_NVIC_SetPriority(UsageFault_IRQn,0,0);


	  __HAL_RCC_SYSCFG_CLK_ENABLE();
	  __HAL_RCC_PWR_CLK_ENABLE();

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htimer)
{

	//1. enable the clock for the TIM6 peripheral
	__HAL_RCC_TIM6_CLK_ENABLE();

	//2. Enable the IRQ of TIM6
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

	//3. setup the priority for TIM6_DAC_IRQn
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn,15,0);

}

void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
	  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_CAN1_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

	/**CAN1 GPIO Configuration
	PA11     ------> CAN1_RX
	PA12     ------> CAN1_TX
	*/

	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF9_CAN;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF9_CAN;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(CAN_TX_IRQn,15,0);
	HAL_NVIC_SetPriority(CAN_RX0_IRQn,15,0);
	HAL_NVIC_SetPriority(CAN_RX1_IRQn,15,0);
	HAL_NVIC_SetPriority(CAN_SCE_IRQn,15,0);

	HAL_NVIC_EnableIRQ(CAN_TX_IRQn);
	HAL_NVIC_EnableIRQ(CAN_RX0_IRQn);
	HAL_NVIC_EnableIRQ(CAN_RX1_IRQn);
	HAL_NVIC_EnableIRQ(CAN_SCE_IRQn);


}
