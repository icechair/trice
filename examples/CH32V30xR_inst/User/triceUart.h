/*! \file triceUart.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_UART_H_
#define TRICE_UART_H_

#include "ch32v30x_usart.h"
#include "core_riscv.h"
#ifdef __cplusplus
extern "C" {
#endif

#include <ch32v30x.h>
#include "trice.h"

TRICE_INLINE void ToggleOpticalFeedbackLED( void ){
    //LL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

#if TRICE_DEFERRED_UARTA == 1

//! Check if a new byte can be written into trice transmit register.
//! \retval 0 == not empty
//! \retval !0 == empty
//! User must provide this function.
TRICE_INLINE uint32_t triceTxDataRegisterEmptyUartA(void) {
    //return LL_USART_IsActiveFlag_TXE(TRICE_UARTA);
    return USART_GetFlagStatus(TRICE_UARTA, USART_FLAG_TXE) == SET;
}

//! Write value v into trice transmit register.
//! \param v byte to transmit
//! User must provide this function.
TRICE_INLINE void triceTransmitData8UartA(uint8_t v) {
    //LL_USART_TransmitData8(TRICE_UARTA, v);
    USART_SendData(TRICE_UARTA, v);
    ToggleOpticalFeedbackLED();
}

//! Allow interrupt for empty trice data transmit register.
//! User must provide this function.
TRICE_INLINE void triceEnableTxEmptyInterruptUartA(void) {
    //LL_USART_EnableIT_TXE(TRICE_UARTA);
    USART_ITConfig(TRICE_UARTA, USART_IT_TXE, ENABLE);
}

//! Disallow interrupt for empty trice data transmit register.
//! User must provide this function.
TRICE_INLINE void triceDisableTxEmptyInterruptUartA(void) {
    //LL_USART_DisableIT_TXE(TRICE_UARTA);
    USART_ITConfig(TRICE_UARTA, USART_IT_TXE, DISABLE);
}
#endif // #if TRICE_DEFERRED_UARTA == 1

#if TRICE_DEFERRED_UARTB == 1

//! Check if a new byte can be written into trice transmit register.
//! \retval 0 == not empty
//! \retval !0 == empty
//! User must provide this function.
TRICE_INLINE uint32_t triceTxDataRegisterEmptyUartB(void) {
    //return LL_USART_IsActiveFlag_TXE(TRICE_UARTB);
    return USART_GetFlagStatus(TRICE_UARTB, USART_FLAG_TXE) == SET;
}

//! Write value v into trice transmit register.
//! \param v byte to transmit
//! User must provide this function.
TRICE_INLINE void triceTransmitData8UartB(uint8_t v) {
    //LL_USART_TransmitData8(TRICE_UARTB, v);
    USART_SendData(TRICE_UARTB, v);
    ToggleOpticalFeedbackLED();
}

//! Allow interrupt for empty trice data transmit register.
//! User must provide this function.
TRICE_INLINE void triceEnableTxEmptyInterruptUartB(void) {
    LL_USART_EnableIT_TXE(TRICE_UARTB);
    USART_ITConfig(TRICE_UARTB, USART_IT_TXE, ENABLE);
}

//! Disallow interrupt for empty trice data transmit register.
//! User must provide this function.
TRICE_INLINE void triceDisableTxEmptyInterruptUartB(void) {
    LL_USART_DisableIT_TXE(TRICE_UARTB);
    USART_ITConfig(TRICE_UARTB, USART_IT_TXE, DISABLE);
}
#endif // #if TRICE_DEFERRED_UARTB == 1

#ifdef __cplusplus
}
#endif

#endif /* TRICE_UART_H_ */
