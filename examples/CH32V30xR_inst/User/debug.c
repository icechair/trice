#include <ch32v30x.h>
#include "debug.h"
#include "ch32v30x_usart.h"
#include "core_riscv.h"

static volatile int* DEBUG_DATA0_ADDRESS = ((volatile int*)0xE0000380);
static volatile int* DEBUG_DATA1_ADDRESS = ((volatile int*)0xE0000384);

#define DEBUG_UART     USART1
#define DEBUG_BAUDRATE 115200

void systick_init()
{
    SysTick->CTLR = 0;
    SysTick->SR = 0;
    SysTick->CNT = 0;
    SysTick->CMP = SystemCoreClock/1000;
    SysTick->CTLR = 0xF;

    NVIC_SetPriority(SysTicK_IRQn, 15);
    NVIC_EnableIRQ(SysTicK_IRQn);
}
void debug_init()
{
    GPIO_InitTypeDef  GPIO_InitStructure  = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef  NVIC_InitStructure  = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    //LED1
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //LED2
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate            = DEBUG_BAUDRATE;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(DEBUG_UART, &USART_InitStructure);
    USART_ITConfig(DEBUG_UART, USART_IT_RXNE, ENABLE);
//    USART_ITConfig(DEBUG_UART, USART_IT_TXE, ENABLE);

    NVIC_Init(&NVIC_InitStructure);
    NVIC_SetPriority(USART1_IRQn, 1);
    NVIC_EnableIRQ(USART1_IRQn);

    USART_Cmd(DEBUG_UART, ENABLE);

    systick_init();
}

void debug_sdi_enable(void)
{
    *DEBUG_DATA0_ADDRESS = 0;
}

void debug_sdi_write(const char* const buf, unsigned const size)
{
    int i         = 0;
    int writeSize = size;
    do
    {
        /**
         * data0  data1 8 bytes
         * data0 The lowest byte storage length, the maximum is 7
         *
         */

        while((*(DEBUG_DATA0_ADDRESS) != 0u))
        {
        }

        if(writeSize > 7)
        {
            *(DEBUG_DATA1_ADDRESS) = (*(buf + i + 3)) | (*(buf + i + 4) << 8) |
                                     (*(buf + i + 5) << 16) | (*(buf + i + 6) << 24);
            *(DEBUG_DATA0_ADDRESS) = (7u) | (*(buf + i) << 8) | (*(buf + i + 1) << 16) |
                                     (*(buf + i + 2) << 24);

            i += 7;
            writeSize -= 7;
        }
        else
        {
            *(DEBUG_DATA1_ADDRESS) = (*(buf + i + 3)) | (*(buf + i + 4) << 8) |
                                     (*(buf + i + 5) << 16) | (*(buf + i + 6) << 24);
            *(DEBUG_DATA0_ADDRESS) = (writeSize) | (*(buf + i) << 8) |
                                     (*(buf + i + 1) << 16) | (*(buf + i + 2) << 24);

            writeSize = 0;
        }

    } while(writeSize);
}
