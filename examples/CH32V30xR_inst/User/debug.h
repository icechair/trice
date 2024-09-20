#ifndef _USER_DEBUG_H_
#define _USER_DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <ch32v30x.h>

/* SDI Printf Definition */
#define SDI_PR_CLOSE 0
#define SDI_PR_OPEN  1

#ifndef SDI_PRINT
    #define SDI_PRINT SDI_PR_CLOSE
#endif

void debug_init();
void debug_sdi_enable(void);
void debug_sdi_write(const char* const buffer, unsigned const size);

#define DEBUG_PIN_1 GPIOA, GPIO_Pin_14
#define DEBUG_PIN_2 GPIOA, GPIO_Pin_15

#define DEBUG_SET_PIN

#ifdef __cplusplus
}
#endif

#endif // _USER_DEBUG_H_
