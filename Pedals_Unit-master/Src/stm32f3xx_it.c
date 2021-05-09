/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f3xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

//================== UNIT ==========================

extern const uint8_t unit_ID;


//=================== CAN 1 =========================

extern volatile unsigned char CAN_1_Rx_received_flg;
extern volatile unsigned char CAN_1_RecData[8];
unsigned char CAN_1_Time_Stemp_420[2];
unsigned char CAN_1_Time_Stemp_80[2];
unsigned long CAN_1_Rx_ID  = 0x00; 
unsigned long CAN_1_Rx_temp_id;
unsigned int  CAN_1_Rx_eid;
unsigned int  CAN_1_Rx_sid;
unsigned long CAN_1_Tx_temp_id;
uint32_t CAN_1_Rx_ide;
uint32_t CAN_1_Rx_rtr;
uint32_t CAN_1_Rx_dlc;
uint32_t CAN_1_Rx_fmi;


//==================== ADC 2 =====================
extern volatile uint16_t buff_adc_2[3];
extern unsigned int time_out_ADC_2;

uint8_t bpps_debounce[10];
uint8_t bpps_signal = 0x01;
uint8_t valid_Apps_Bpps = 0x00;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
        
inline void samp_APPS1_APPS2( void );  
inline void debounce_BPPS( void ); 

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern ADC_HandleTypeDef hadc2;
extern CAN_HandleTypeDef hcan;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F3xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f3xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles ADC1 and ADC2 interrupts.
  */
void ADC1_2_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_2_IRQn 0 */
  
  if ( __HAL_ADC_GET_FLAG( &hadc2 , ADC_FLAG_EOC) ){
    
      asm("NOP");
      asm("NOP");
      asm("NOP");
      asm("NOP");
    
  }
  
  
  if ( __HAL_ADC_GET_FLAG( &hadc2 , ADC_FLAG_EOS) ){

    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");

  }
  
  
  
  if ( __HAL_ADC_GET_FLAG( &hadc2 , ADC_FLAG_OVR) ){

    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");

  }

#if 1
  /* USER CODE END ADC1_2_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc2);
  /* USER CODE BEGIN ADC1_2_IRQn 1 */
#endif
  /* USER CODE END ADC1_2_IRQn 1 */
}

/**
  * @brief This function handles CAN TX interrupt.
  */
void CAN_TX_IRQHandler(void)
{
  /* USER CODE BEGIN CAN_TX_IRQn 0 */

  
  
  
  
  
#if 0
  /* USER CODE END CAN_TX_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan);
  /* USER CODE BEGIN CAN_TX_IRQn 1 */
#endif
  
  
  
  
  /* USER CODE END CAN_TX_IRQn 1 */
}

/**
  * @brief This function handles CAN RX0 interrupt.
  */
void CAN_RX0_IRQHandler(void)
{
  /* USER CODE BEGIN CAN_RX0_IRQn 0 */
  
  
  
  uint32_t errorcode = HAL_CAN_ERROR_NONE;
  uint32_t interrupts = READ_REG(CAN->IER);
  uint32_t msrflags = READ_REG(CAN->MSR);
  uint32_t tsrflags = READ_REG(CAN->TSR);
  uint32_t rf0rflags = READ_REG(CAN->RF0R);
  uint32_t rf1rflags = READ_REG(CAN->RF1R);
  uint32_t esrflags = READ_REG(CAN->ESR);
  
  /* Receive FIFO 0 message pending interrupt management *********************/
  if ((interrupts & CAN_IT_RX_FIFO0_MSG_PENDING) != 0U)
  {
    /* Check if message is still pending */
    if ((CAN->RF0R & CAN_RF0R_FMP0) != 0U)
    {
      CAN_1_Rx_ide = (uint8_t)0x04U & CAN->sFIFOMailBox[0].RIR;
      if ( CAN_1_Rx_ide == CAN_ID_STD )
      {
        CAN_1_Rx_eid = 0x00;
        CAN_1_Rx_sid = 0x000007FFU & (CAN->sFIFOMailBox[0].RIR >> 21U);
      }
      else
      {
        CAN_1_Rx_sid = 0x00;
        CAN_1_Rx_eid = 0x1FFFFFFFU & (CAN->sFIFOMailBox[0].RIR >> 3U);
      }
      if( CAN_1_Rx_sid == 0x420 ){
        

        samp_APPS1_APPS2();
        debounce_BPPS();
     
        CAN->sTxMailBox[0U].TIR =  (( (0x420 + unit_ID ) << 21U) |  0);
        /* Set up the DLC */
        CAN->sTxMailBox[0U].TDTR &= 0xFFFFFFF0U;                                               
        CAN->sTxMailBox[0U].TDTR |= 0x00000008U;            
        /* Set up the data field */
        CAN->sTxMailBox[0U].TDLR =  ( ( buff_adc_2[1] << 16U) | buff_adc_2[0] );
        CAN->sTxMailBox[0U].TDHR =  (valid_Apps_Bpps << 24U) |  (bpps_signal << 16U) | buff_adc_2[2] ;
        /* Request transmission */ 
        CAN->sTxMailBox[0U].TIR  |=  CAN_TI0R_TXRQ;        
        
        
        
      }
      else if( CAN_1_Rx_sid == 0x80 ){

        
        CAN->sTxMailBox[1U].TIR =  (( ( 0x80 + unit_ID ) << 21U) |  0);
        /* Set up the DLC */
        CAN->sTxMailBox[1U].TDTR &= 0xFFFFFFF0U;                                               
        CAN->sTxMailBox[1U].TDTR |= 0x00000008U;            
        /* Set up the data field */
        CAN->sTxMailBox[1U].TDLR =  (0x55 << 24U) |  (0x55 << 16U) |(0x55 << 8U) | (0x55 );
        CAN->sTxMailBox[1U].TDHR =  (0x55 << 24U) |  (0x55 << 16U) |(0x55 << 8U) | (0x55 );
        /* Request transmission */ 
        CAN->sTxMailBox[1U].TIR  |=  CAN_TI0R_TXRQ; 
        }
      

    SET_BIT(CAN->RF0R, CAN_RF0R_RFOM0);
    }
  }
  
#if 0
  if ( __HAL_CAN_GET_FLAG( &hcan , CAN_RF0R_FOVR0 ) ){            //???
    
      CAN_1_Rx_ide = (uint8_t)0x04U & CAN->sFIFOMailBox[0].RIR;
      if ( CAN_1_Rx_ide == CAN_ID_STD )
      {
        CAN_1_Rx_eid = 0x00;
        CAN_1_Rx_sid = 0x000007FFU & (CAN->sFIFOMailBox[0].RIR >> 21U);
      }
      else
      {
        CAN_1_Rx_sid = 0x00;
        CAN_1_Rx_eid = 0x1FFFFFFFU & (CAN->sFIFOMailBox[0].RIR >> 3U);
      }

        CAN_1_Rx_ID = CAN_1_Rx_eid;
        CAN_1_Rx_ID <<= 12;
        CAN_1_Rx_ID |= CAN_1_Rx_sid;
      
       CAN_1_Rx_rtr = (uint8_t)0x02U & CAN->sFIFOMailBox[0].RIR;
       CAN_1_Rx_dlc = (uint8_t)0x0FU & CAN->sFIFOMailBox[0].RDTR;
       CAN_1_Rx_fmi = (uint8_t)0xFFU & (CAN->sFIFOMailBox[0].RDTR >> 8U);

       CAN_1_RecData[0] = CAN->sFIFOMailBox[0].RDLR;
       CAN_1_RecData[1] = CAN->sFIFOMailBox[0].RDLR >> (8U);
       CAN_1_RecData[2] = CAN->sFIFOMailBox[0].RDLR >> (16U);
       CAN_1_RecData[3] = CAN->sFIFOMailBox[0].RDLR >> (24U);
       CAN_1_RecData[4] = CAN->sFIFOMailBox[0].RDHR;
       CAN_1_RecData[5] = CAN->sFIFOMailBox[0].RDHR >> (8U);
       CAN_1_RecData[6] = CAN->sFIFOMailBox[0].RDHR >> (16U);
       CAN_1_RecData[7] = CAN->sFIFOMailBox[0].RDHR >> (24U);
    
    
    __HAL_CAN_CLEAR_FLAG( &hcan , CAN_RF0R_FOVR0 );
    }
#endif  
#if 0
  /* USER CODE END CAN_RX0_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan);
  /* USER CODE BEGIN CAN_RX0_IRQn 1 */
#endif
  /* USER CODE END CAN_RX0_IRQn 1 */
}

/**
  * @brief This function handles CAN RX1 interrupt.
  */
void CAN_RX1_IRQHandler(void)
{
  /* USER CODE BEGIN CAN_RX1_IRQn 0 */
  
    if ( __HAL_CAN_GET_FLAG( &hcan , CAN_FLAG_FF1 ) ){
    
    asm("NOP");
    
    
    
    __HAL_CAN_CLEAR_FLAG( &hcan , CAN_FLAG_FF1 );
  }
#if 0
  /* USER CODE END CAN_RX1_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan);
  /* USER CODE BEGIN CAN_RX1_IRQn 1 */
#endif
  /* USER CODE END CAN_RX1_IRQn 1 */
}

/**
  * @brief This function handles CAN SCE interrupt.
  */
void CAN_SCE_IRQHandler(void)
{
  /* USER CODE BEGIN CAN_SCE_IRQn 0 */

  /* USER CODE END CAN_SCE_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan);
  /* USER CODE BEGIN CAN_SCE_IRQn 1 */

  /* USER CODE END CAN_SCE_IRQn 1 */
}

/* USER CODE BEGIN 1 */


inline void samp_APPS1_APPS2( void ){   

    time_out_ADC_2 = 0;
    ADC2->CR |= ( ADC_CR_ADSTART  );
    
    while( (!( ADC2->ISR &   ADC_ISR_EOC )) && ( time_out_ADC_2 < TIME_OUT_ADC_2 ) ){     
      time_out_ADC_2++;  
    }  
    buff_adc_2[0] = ADC2->DR;  
    ADC2->ISR &= ~ADC_ISR_EOC;
    
    while( (!( ADC2->ISR &   ADC_ISR_EOC )) && ( time_out_ADC_2 < TIME_OUT_ADC_2 ) ){
      time_out_ADC_2++;  
    }  
    buff_adc_2[1] = ADC2->DR;
    ADC2->ISR &= ~ADC_ISR_EOC; 
 
    while( (!( ADC2->ISR &   ADC_ISR_EOC )) && ( time_out_ADC_2 < TIME_OUT_ADC_2 ) ){
      time_out_ADC_2++;  
    }  
    buff_adc_2[2] = ADC2->DR;
    ADC2->ISR &= ~ADC_ISR_EOC;
  
    if ( time_out_ADC_2 >= TIME_OUT_ADC_2 ){
      
      buff_adc_2[0] = ERROR_APPS;
      buff_adc_2[1] = ERROR_APPS;
      buff_adc_2[2] = ERROR_APPS;
      valid_Apps_Bpps = ERROR_APPS_BPPS;
    }
    

  
  
}


inline void debounce_BPPS( void ){ 


    bpps_signal = 0x00;
    for(int j = 0 ; j < 10 ; j++ ){
      
      bpps_debounce[j] = HAL_GPIO_ReadPin( BPPS_GPIO_Port  , BPPS_Pin);
      bpps_signal = bpps_signal + bpps_debounce[j];
      
      
    }
    
    if( bpps_signal > 0x08){
      
      bpps_signal = 0x01;
    }
    else if( bpps_signal < 0x02){
      
      bpps_signal = 0x00;
    }
    else{
      bpps_signal = ERROR_BPPS;
      valid_Apps_Bpps = ERROR_APPS_BPPS;
     
    }
        
}

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
