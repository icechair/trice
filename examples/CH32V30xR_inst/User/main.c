/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 USART interrupt routine:
 Master:USART2_Tx(PA2)\USART2_Rx(PA3).
 Slave:USART3_Tx(PB10)\USART3_Rx(PB11).

 This example demonstrates that UART2 and USART3 use
 query to send and interrupt to receive.

     Hardware connection:
               PA2 -- PB11
               PA3 -- PB10

*/
#include <stdint.h>
#include "ch32v30x.h"
#include "trice.h"
/* Global define */
#define TxSize1 (size(TxBuffer1))
#define TxSize2 (size(TxBuffer2))
#define size(a) (sizeof(a) / sizeof(*(a)))

/* Global typedef */
typedef enum
{
    FAILED = 0,
    PASSED = !FAILED
} TestStatus;

/* Global Variable */
uint8_t TxBuffer1[] =
    "*Buffer1 Send from USART2 to USART3 using Interrupt!"; /* Send by UART2 */
uint8_t TxBuffer2[] =
    "#Buffer2 Send from USART3 to USART2 using Interrupt!"; /* Send by UART3 */
uint8_t RxBuffer1[TxSize1] = {0};                           /* USART2 Using */
uint8_t RxBuffer2[TxSize2] = {0};                           /* USART3 Using  */

volatile uint8_t TxCnt1 = 0, RxCnt1 = 0;
volatile uint8_t TxCnt2 = 0, RxCnt2 = 0;

volatile uint8_t Rxfinish1 = 0, Rxfinish2 = 0;

TestStatus TransferStatus1 = FAILED;
TestStatus TransferStatus2 = FAILED;

void USART2_IRQHandler(void) __attribute__((interrupt()));
void USART3_IRQHandler(void) __attribute__((interrupt()));

/*********************************************************************
 * @fn      Buffercmp
 *
 * @brief   Compares two buffers
 *
 * @param   Buf1,Buf2 - buffers to be compared
 *          BufferLength - buffer's length
 *
 * @return  PASSED - Buf1 identical to Buf
 *          FAILED - Buf1 differs from Buf2
 */
TestStatus Buffercmp(uint8_t* Buf1, uint8_t* Buf2, uint16_t BufLength)
{
    while(BufLength--)
    {
        if(*Buf1 != *Buf2)
        {
            return FAILED;
        }
        Buf1++;
        Buf2++;
    }
    return PASSED;
}

/*********************************************************************
 * @fn      USARTx_CFG
 *
 * @brief   Initializes the USART2 & USART3 peripheral.
 *
 * @return  none
 */
void USARTx_CFG(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure  = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef  NVIC_InitStructure  = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    /* USART2 TX-->A.2   RX-->A.3 */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* USART3 TX-->B.10  RX-->B.11 */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel                   = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART2, ENABLE);
    USART_Cmd(USART3, ENABLE);
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    TriceInit();
        trice( iD(7515), "\n\n        ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨        \n        🎈🎈🎈🎈  𝕹𝖀𝕮𝕷𝕰𝕺-F030R8   🎈🎈🎈🎈\n        🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃        \n\n\n");

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    debug_init();
    USARTx_CFG(); /* USART2 & USART3 INIT */
#if TRICE_RING_BUFFER_OVERFLOW_WATCH == 1
    TriceInitRingBufferMargins();
#endif
    Trice( iD(4863), "wrn: Hello! 👋🙂 \a\n"); // with sound!
    Trice( iD(3136), "wrn: Hello! 👋🙂 \a\n"); // with sound!
    Trice( iD(2893), "wrn: Hello! 👋🙂 \a\n"); // with sound!
    TRice( iD(7685), "wrn: Hello! 👋🙂 \a\n"); // with sound!
    TRice( iD(2602), "wrn: Hello! 👋🙂 \a\n"); // with sound!
    Trice( iD(6252), "wrn: Hello! 👋🙂 \a\n"); // with sound!
    Trice( iD(4651), "wrn: Hello! 👋🙂 \a\n"); // with sound!
    Trice( iD(2471), "wrn: Hello! 👋🙂 \a\n"); // with sound!
    Trice( iD(7323), "wrn: Hello! 👋🙂 \a\n"); // with sound!
    trice( iD(2169), "wrn: Hello! 👋🙂 \a\n"); // with sound!
    trice( iD(7655), "wrn: Hello! 👋🙂 \a\n"); // with sound!

    trice( iD(5158), "SystemClk:%ld\r\n", SystemCoreClock);
    trice( iD(5946), "ChipID:%08lx\r\n", DBGMCU_GetCHIPID());
    trice( iD(1282), "USART Interrupt TEST\r\n");
    while(TxCnt2 < TxSize2) /* USART3--->USART2 */
    {
        USART_SendData(USART3, TxBuffer2[TxCnt2++]);
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) ==
              RESET) /* waiting for sending finish */
        {
        }
    }
    while(TxCnt1 < TxSize1) /* USART2--->USART3 */
    {
        USART_SendData(USART2, TxBuffer1[TxCnt1++]);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) ==
              RESET) /* waiting for sending finish */
        {
        }
    }

    while(!Rxfinish1 || !Rxfinish2) /* waiting for receiving int finish */
    {
    }

    TransferStatus1 = Buffercmp(TxBuffer1, RxBuffer2, TxSize1);
    TransferStatus2 = Buffercmp(TxBuffer2, RxBuffer1, TxSize2);

    if(TransferStatus1 && TransferStatus2)
    {
        trice( iD(5641), "\r\nSend Success!\r\n");
    }
    else
    {
        trice( iD(7196), "\r\nSend Fail!\r\n");
    }
    trice( iD(2712), "TxBuffer1---->RxBuffer2     TxBuffer2---->RxBuffer1\r\n");
    triceS( iD(2424), "TxBuffer1:%s\r\n", TxBuffer1);
    triceS( iD(4407), "RxBuffer1:%s\r\n", RxBuffer1);
    triceS( iD(6322), "TxBuffer2:%s\r\n", TxBuffer2);
    triceS( iD(1981), "RxBuffer2:%s\r\n", RxBuffer2);

    static volatile uint32_t lastMs = 0;
    while(1)
    {
        if(lastMs != ms32)
        { // each ms
            lastMs = ms32;

            static uint32_t msDiag = 0;
            msDiag++;
            if(msDiag >= 3000)
            {
                msDiag = 0;
                //TriceLogDiagnosticValues();
            }

            static uint32_t msCheck = 0;
            msCheck++;
            if(msCheck >= 10)
            {
                msCheck           = 0;
                static uint32_t i = 0;
                if(i++ > 400)
                {
                    i = 0;
                }
                TriceCheck(i); // this generates trice data
            }

#if(TRICE_BUFFER == TRICE_RING_BUFFER) || (TRICE_BUFFER == TRICE_DOUBLE_BUFFER)

            static uint32_t msTransfer = 0;
            msTransfer++;
            if(msTransfer >= 1)
            {
                msTransfer = 0;
                // Serve deferred trice transfer every few ms or if TRICE_BUFFER is getting filled. With an RTOS put this in a separate task.
                // In TRICE_RING_BUFFER && TRICE_SINGLE_PACK_MODE TriceTransfer can transmit only 1 Trice per call, so call it every 1ms then.
                // In TRICE_DOUBLE_BUFFER TriceTransfer can transmit one half buffer.
                // TRICE_RING_BUFFER && TRICE_MULTI_PACK_MODE not implemented
                TriceTransfer(); // serve deferred output
                // For the ring buffer TriceTransfer transmits only one Trice per call, so doing that each ms is ok.
            }
    #if TRICE_RING_BUFFER_OVERFLOW_WATCH == 1
            WatchRingBufferMargins();
    #endif

#endif // #if ( TRICE_BUFFER == TRICE_RING_BUFFER) || ( TRICE_BUFFER == TRICE_DOUBLE_BUFFER)
        }
    }
}

/*********************************************************************
 * @fn      USART2_IRQHandler
 *
 * @brief   This function handles USART2 global interrupt request.
 *
 * @return  none
 */
void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        RxBuffer1[RxCnt1++] = USART_ReceiveData(USART2);

        if(RxCnt1 == TxSize2)
        {
            USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
            Rxfinish1 = 1;
        }
    }
}

/*********************************************************************
 * @fn      USART3_IRQHandler
 *
 * @brief   This function handles USART3 global interrupt request.
 *
 * @return  none
 */
void USART3_IRQHandler(void)
{
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        RxBuffer2[RxCnt2++] = USART_ReceiveData(USART3);

        if(RxCnt2 == TxSize1)
        {
            USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
            Rxfinish2 = 1;
        }
    }
}
