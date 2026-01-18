#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>

// Kích thước bộ đệm. 512 byte là đủ cho các lệnh JSON thông thường.
#define UART_BUFFER_SIZE 512

typedef struct {
    uint8_t buffer[UART_BUFFER_SIZE];
    volatile uint16_t head; // Đầu vào (nơi WiFi đổ dữ liệu vào)
    volatile uint16_t tail; // Đầu ra (nơi chúng ta lấy dữ liệu ra xử lý)
} RingBuffer;

void RB_Init(RingBuffer *rb);
void RB_Put(RingBuffer *rb, uint8_t data);
int RB_Get(RingBuffer *rb, uint8_t *data);
int RB_IsEmpty(RingBuffer *rb);
void RB_Flush(RingBuffer *rb);

#endif
