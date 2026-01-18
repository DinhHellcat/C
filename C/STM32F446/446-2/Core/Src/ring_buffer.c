#include "ring_buffer.h"

// Khởi tạo buffer sạch sẽ
void RB_Init(RingBuffer *rb) {
    rb->head = 0;
    rb->tail = 0;
}

// Đẩy 1 ký tự vào thùng (Dùng trong ngắt UART)
void RB_Put(RingBuffer *rb, uint8_t data) {
    uint16_t next_head = (rb->head + 1) % UART_BUFFER_SIZE;
    if (next_head != rb->tail) { // Chỉ ghi nếu thùng chưa đầy
        rb->buffer[rb->head] = data;
        rb->head = next_head;
    }
}

// Lấy 1 ký tự ra khỏi thùng (Dùng trong vòng lặp chính)
int RB_Get(RingBuffer *rb, uint8_t *data) {
    if (rb->head == rb->tail) {
        return 0; // Thùng rỗng, không có gì để lấy
    }
    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % UART_BUFFER_SIZE;
    return 1; // Lấy thành công
}

// Kiểm tra xem thùng có rỗng không
int RB_IsEmpty(RingBuffer *rb) {
    return (rb->head == rb->tail);
}

// Xóa sạch thùng (reset)
void RB_Flush(RingBuffer *rb) {
    rb->head = 0;
    rb->tail = 0;
}
