/********************************** (C) COPYRIGHT *******************************
* File Name          : ch32v30x_it.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2024/03/06
* Description        : Main Interrupt Service Routines.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#include <stdint.h>
#include <string.h>
#include "ch32v30x.h"
#include "trice.h"
#include "debug.h"

void NMI_Handler(void) __attribute__((interrupt()));
void HardFault_Handler(void) __attribute__((interrupt()));
void SysTick_Handler(void) __attribute__((interrupt()));
void USART1_IRQHandler(void) __attribute__((interrupt()));

/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
void NMI_Handler(void)
{
    while(1)
    {
    }
}

/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
void HardFault_Handler(void)
{
    while(1)
    {
    }
}

volatile uint32_t ms32;

void SysTick_Handler(void)
{
    ms32 += 1;
    SysTick->SR = 0;
    if(ms32 % 500 == 0){
        GPIO_SetBits(DEBUG_PIN_1);
        GPIO_ResetBits(DEBUG_PIN_2);
    }else{
        GPIO_ResetBits(DEBUG_PIN_1);
        GPIO_SetBits(DEBUG_PIN_2);

    }
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(TRICE_UARTA, USART_IT_RXNE) == SET)
    {
        static char rxBuf[TRICE_COMMAND_SIZE_MAX + 1];
        static int  index = 0;
        if(USART_GetFlagStatus(TRICE_UARTA, USART_FLAG_ORE))
        {
            TRice( iD(2260), "WARNING:USARTq OverRun Error Flag is set!\n");
            uint8_t v    = USART_ReceiveData(TRICE_UARTA);
            rxBuf[index] = (char)v;
            index += index < TRICE_COMMAND_SIZE_MAX ? 1 : 0;
            if(v == 0)
            {
                trice( iD(6272), "rx:received command:%s\n", rxBuf);
                strcpy(triceCommandBuffer, rxBuf);
                triceCommandFlag = 1;
                index            = 0;
            }
            return;
        }
    }

    if(USART_GetITStatus(TRICE_UARTA, USART_IT_TXE) == SET)
    {
        triceServeTransmitUartA();
        return;
    }
}
