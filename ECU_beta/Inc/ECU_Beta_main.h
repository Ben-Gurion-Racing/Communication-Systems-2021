/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2020 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"


void Error_Handler(void);


/* Private defines -----------------------------------------------------------*/
#define 	USER_Btn_Pin 					GPIO_PIN_13
#define 	USER_Btn_GPIO_Port 				GPIOC
#define 	MCO_Pin 						GPIO_PIN_0
#define 	MCO_GPIO_Port 					GPIOH
#define 	RMII_MDC_Pin 					GPIO_PIN_1
#define 	RMII_MDC_GPIO_Port 				GPIOC
#define 	RMII_REF_CLK_Pin 				GPIO_PIN_1
#define 	RMII_REF_CLK_GPIO_Port 			GPIOA
#define 	RMII_MDIO_Pin 					GPIO_PIN_2
#define 	RMII_MDIO_GPIO_Port 			GPIOA
#define 	RMII_CRS_DV_Pin 				GPIO_PIN_7
#define 	RMII_CRS_DV_GPIO_Port 			GPIOA
#define 	RMII_RXD0_Pin 					GPIO_PIN_4
#define 	RMII_RXD0_GPIO_Port 			GPIOC
#define 	RMII_RXD1_Pin 					GPIO_PIN_5
#define 	RMII_RXD1_GPIO_Port 			GPIOC
#define 	relay_Pin 						GPIO_PIN_10
#define 	relay_GPIO_Port 				GPIOB
#define 	RMII_TXD1_Pin 					GPIO_PIN_13
#define 	RMII_TXD1_GPIO_Port 			GPIOB
#define 	LD3_Pin 						GPIO_PIN_14
#define 	LD3_GPIO_Port 					GPIOB
#define 	STLK_RX_Pin 					GPIO_PIN_8
#define 	STLK_RX_GPIO_Port 				GPIOD
#define 	STLK_TX_Pin 					GPIO_PIN_9
#define 	STLK_TX_GPIO_Port 				GPIOD
#define 	USB_PowerSwitchOn_Pin 			GPIO_PIN_6
#define 	USB_PowerSwitchOn_GPIO_Port	 	GPIOG
#define 	USB_OverCurrent_Pin 			GPIO_PIN_7
#define 	USB_OverCurrent_GPIO_Port 		GPIOG
#define 	USB_SOF_Pin 					GPIO_PIN_8
#define 	USB_SOF_GPIO_Port 				GPIOA
#define 	USB_VBUS_Pin 					GPIO_PIN_9
#define 	USB_VBUS_GPIO_Port 				GPIOA
#define 	USB_ID_Pin 						GPIO_PIN_10
#define 	USB_ID_GPIO_Port 				GPIOA
#define 	USB_DM_Pin 						GPIO_PIN_11
#define 	USB_DM_GPIO_Port 				GPIOA
#define 	USB_DP_Pin 						GPIO_PIN_12
#define 	USB_DP_GPIO_Port 				GPIOA
#define 	TMS_Pin 						GPIO_PIN_13
#define 	TMS_GPIO_Port 					GPIOA
#define 	TCK_Pin 						GPIO_PIN_14
#define 	TCK_GPIO_Port 					GPIOA
#define 	RMII_TX_EN_Pin 					GPIO_PIN_11
#define 	RMII_TX_EN_GPIO_Port 			GPIOG
#define 	RMII_TXD0_Pin 					GPIO_PIN_13
#define 	RMII_TXD0_GPIO_Port 			GPIOG
#define 	SWO_Pin 						GPIO_PIN_3
#define 	SWO_GPIO_Port 					GPIOB
#define 	LD2_Pin 						GPIO_PIN_7
#define 	LD2_GPIO_Port 					GPIOB
#define 	R2D_Pin 						GPIO_PIN_0
#define 	R2D_GPIO_Port 					GPIOE
#define 	SCALE_10 						10
#define 	SCALE_20 						20
#define 	FULL_SCALE 						100
#define 	NEUTRAL 						1
#define 	DRIVE 							2
#define 	TRAINER 						3
#define 	REST 							0
#define 	Torques_LED 					4
#define 	Brakes_LED 					    5
#define     WARNING							6
#define     SHUTDOWN						7
#define 	TORQUE_LED1_LOW 				1
#define 	TORQUE_LED2_LOW 			    10
#define 	TORQUE_LED3_LOW 			    20
#define 	TORQUE_LED4_LOW 				30
#define 	TORQUE_LED5_LOW 			    40
#define 	TORQUE_LED6_LOW 			    50
#define 	TORQUE_LED7_LOW 				60
#define 	TORQUE_LED8_LOW 			    70
#define 	TORQUE_LED9_LOW 			    80
#define 	TORQUE_LED10_LOW 			    90
#define 	TORQUE_ALL_LEDS                 100
#define 	BRAKES_LED1_LOW 				1
#define 	BRAKES_LED2_LOW 			    10
#define 	BRAKES_LED3_LOW 			    20
#define 	BRAKES_LED4_LOW 				30
#define 	BRAKES_LED5_LOW 			    40
#define 	BRAKES_LED6_LOW 			    50
#define 	BRAKES_LED7_LOW 				60
#define 	BRAKES_LED8_LOW 			    70
#define 	BRAKES_LED9_LOW 			    80
#define 	BRAKES_LED10_LOW 			    90
#define 	BRAKES_ALL_LEDS                 100

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

