
#include <Data_Logger_main.h>

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
	HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);

	/* DebugMonitor_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);

	/* PendSV_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM6)
  {
	//1. enable the clock for the TIM6 peripheral
	__HAL_RCC_TIM6_CLK_ENABLE();

	//2. Enable the IRQ of TIM6
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

	//3. setup the priority for TIM6_DAC_IRQn
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn,15,0);
  }

  else if(htim_base->Instance==TIM7)
  {
	//1. enable the clock for the TIM7 peripheral
	__HAL_RCC_TIM7_CLK_ENABLE();

	//2. Enable the IRQ of TIM7
	HAL_NVIC_EnableIRQ(TIM7_IRQn);

	//3. setup the priority for TIM7_IRQn
	HAL_NVIC_SetPriority(TIM7_IRQn,15,0);

  }
}

void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if(hcan->Instance == CAN1)
  {
     __HAL_RCC_CAN1_CLK_ENABLE();
     __HAL_RCC_GPIOD_CLK_ENABLE();

    /**CAN1 GPIO Configuration
    PD0     ------> CAN1_RX
    PD1     ------> CAN1_TX   */

    GPIO_InitStruct.Pin 					= GPIO_PIN_0;
    GPIO_InitStruct.Mode 					= GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull 					= GPIO_NOPULL;
    GPIO_InitStruct.Speed 					= GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate 				= GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin 					= GPIO_PIN_1;
    GPIO_InitStruct.Mode 					= GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull 					= GPIO_NOPULL;
    GPIO_InitStruct.Speed 					= GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate 				= GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(CAN1_TX_IRQn,15,0);
	HAL_NVIC_SetPriority(CAN1_RX0_IRQn,15,0);
	HAL_NVIC_SetPriority(CAN1_RX1_IRQn,15,0);
	HAL_NVIC_SetPriority(CAN1_SCE_IRQn,15,0);

	HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
  }

  else if(hcan->Instance == CAN2)
  {
    __HAL_RCC_CAN2_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /**CAN2 GPIO Configuration
    PB5     ------> CAN2_RX
    PB6     ------> CAN2_TX    */

    GPIO_InitStruct.Pin 					= GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode 					= GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull 					= GPIO_NOPULL;
    GPIO_InitStruct.Speed 					= GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate 				= GPIO_AF9_CAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(CAN2_TX_IRQn,15,0);
	HAL_NVIC_SetPriority(CAN2_RX0_IRQn,15,0);
	HAL_NVIC_SetPriority(CAN2_RX1_IRQn,15,0);
	HAL_NVIC_SetPriority(CAN2_SCE_IRQn,15,0);

	HAL_NVIC_EnableIRQ(CAN2_TX_IRQn);
	HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
	HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);
	HAL_NVIC_EnableIRQ(CAN2_SCE_IRQn);
  }
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if(huart->Instance==USART3)
  {
    /* Peripheral clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX   */

    GPIO_InitStruct.Pin 					= STLK_RX_Pin|STLK_TX_Pin;
    GPIO_InitStruct.Mode 					= GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull 					= GPIO_NOPULL;
    GPIO_InitStruct.Speed 					= GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate 				= GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  }
}
