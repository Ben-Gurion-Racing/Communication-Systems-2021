#include <Data_Logger_main.h>
#include "stm32f7xx_it.h"

extern TIM_HandleTypeDef htimer6;
extern TIM_HandleTypeDef htimer7;
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
extern ETH_HandleTypeDef heth;


void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM6_DAC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htimer6);
}

void TIM7_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htimer7);
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

void CAN2_TX_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hcan2);
}

void CAN2_RX1_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hcan2);
}

void CAN2_SCE_IRQHandler(void)
{

	HAL_CAN_IRQHandler(&hcan2);
}

void ETH_IRQHandler(void)
{
	HAL_ETH_IRQHandler(&heth);
}

void EXTI15_10_IRQHandler(void)
{

	HAL_TIM_Base_Start_IT(&htimer6);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}

void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void EXTI2_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

void EXTI4_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}
