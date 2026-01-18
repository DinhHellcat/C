/**
  ******************************************************************************
  * @file    dwt.h
  * @brief   Fixed Header for DWT
  ******************************************************************************
  */
#ifndef DWT_H
#define DWT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
// SỬA: Không include main.h
#include "stm32f4xx_hal.h"

/* External Function ---------------------------------------------------------*/
void DwtInit(void);
void DwtDelay_us(uint32_t usec);
void DwtDelay_ms(uint32_t msec);

#ifdef __cplusplus
}
#endif

#endif /* DWT_H */
