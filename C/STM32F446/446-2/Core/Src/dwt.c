/**
  ******************************************************************************
  * @file    dwt.c
  * @brief   This file includes the utilities for DWT (Fixed for F446)
  ******************************************************************************
  */
#include "dwt.h"

/* Private Define (Khôi phục lại các định nghĩa thanh ghi) */
#define  DWT_LAR_UNLOCK		(uint32_t)0xC5ACCE55
#define  DEM_CR_TRCENA		(1 << 24)
#define  DWT_CR_CYCCNTENA	(1 <<  0)

// Định nghĩa con trỏ trực tiếp đến thanh ghi
#define  DWT_CR				*(volatile uint32_t *)0xE0001000
#define  DWT_LAR			*(volatile uint32_t *)0xE0001FB0
#define  DWT_CYCCNT			*(volatile uint32_t *)0xE0001004
#define  DEM_CR				*(volatile uint32_t *)0xE000EDFC

static uint32_t SysCClk, start;

/**
  * @brief  Initialize DWT
  */
void DwtInit(void)
{
	// Tính toán SysCClk dựa trên Clock hệ thống (đơn vị MHz)
	SysCClk 		= (SystemCoreClock / 1000000);

	// Mở khóa DWT (nếu cần)
	DWT_LAR			|= DWT_LAR_UNLOCK;

	// Bật Trace
	DEM_CR			|= (uint32_t)DEM_CR_TRCENA;

	// Reset bộ đếm
	DWT_CYCCNT		= (uint32_t)0u;

	// Bật bộ đếm Cycle Counter
	DWT_CR			|= (uint32_t)DWT_CR_CYCCNTENA;
}

/**
  * @brief  Start DWT Counter
  */
void DwtStart(void)
{
	start = DWT_CYCCNT;
}

/**
  * @brief  Calculate Interval Base On Previous Start Time
  * @retval Interval in us
  */
float DwtInterval(void)
{
	return (float)(DWT_CYCCNT - start) / SysCClk;
}

/**
  * @brief  Function to delay in microsecond
  * @param	usec	Period in microsecond
  */
void DwtDelay_us(uint32_t usec)
{
	start = DWT_CYCCNT;
	// Chờ cho đến khi số xung đếm được đủ cho thời gian yêu cầu
	while(((DWT_CYCCNT - start) / SysCClk) < usec) {};
}

/**
  * @brief  Function to delay in millisecond
  * @param	msec	Period in millisecond
  */
void DwtDelay_ms(uint32_t msec)
{
	start = DWT_CYCCNT;
	while(((DWT_CYCCNT - start) / SysCClk) < (msec * 1000)) {};
}
