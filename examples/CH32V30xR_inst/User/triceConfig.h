/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <ch32v30x.h>

#define TRICE_CLEAN 0 // If found, this is an automatically modified value.

#define TRICE_DIAGNOSTICS 1

//! ms32 is a 32-bit millisecond counter, counting circular in steps of 1 every ms.
extern volatile uint32_t ms32;
#define TriceStamp16 (SysTick->CNT) // Counts from 31999 -> 0 in each ms.
#define TriceStamp32 ms32
#define TRICE_UARTA  USART1

//#define TRICE_BUFFER TRICE_RING_BUFFER

// trice l -p jlink -args "-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0" -pf NONE -d16 -showID "deb:%04x"
// #define TRICE_DIRECT_OUTPUT 1
// #define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1

// trice l -p com5 -pf cobs -pw MySecret -showID "d:%5d" -ts16 "time:     #%6d"
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_DEFERRED_BUFFER_SIZE 2048
#define TRICE_DATA_OFFSET 16
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS
#define TRICE_DEFERRED_UARTA       1

#define TRICE_ENTER_CRITICAL_SECTION                                                     \
    {                                                                                    \
        __disable_irq();
#define TRICE_LEAVE_CRITICAL_SECTION                                                     \
    __enable_irq();                                                                      \
    }

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
