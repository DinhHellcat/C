//1. Truy cập thanh ghi phần cứng
//====================================================
/*
#include <stdint.h>

// Định nghĩa địa chỉ của thanh ghi GPIO (ví dụ)
#define GPIO_PORTA  ((volatile uint32_t *)0x40004000)

int main(void) {
    // Sử dụng con trỏ để ghi giá trị vào thanh ghi
    *GPIO_PORTA = 0xFF; // Đặt tất cả các bit ở PORTA thành 1
    return 0;
}
*/

//con trỏ thường được dùng để truy cập trực tiếp vào các thanh ghi (registers) của vi điều khiển (microcontroller).
//====================================================

//2. Truyền tham chiếu tới hàm
//====================================================
/*
void swap(uint8_t *a, uint8_t *b) {
    uint8_t temp = *a;
    *a = *b;
    *b = temp;
}

int main(void) {
    uint8_t x = 10, y = 20;
    swap(&x, &y); // Truyền địa chỉ của x và y
    // Sau khi gọi hàm, x = 20, y = 10
    return 0;
}
*/
//====================================================

//3. Mảng và con trỏ
//====================================================
/*
#include <stdint.h>

void clear_buffer(uint8_t *buffer, uint8_t size) {
    for (uint8_t i = 0; i < size; i++) {
        *(buffer + i) = 0; // Xóa từng phần tử trong buffer
    }
}

int main(void) {
    uint8_t data[5] = {1, 2, 3, 4, 5};
    clear_buffer(data, 5); // Truyền mảng như một con trỏ
    // Sau khi gọi hàm, data = {0, 0, 0, 0, 0}
    return 0;
}
*/
//====================================================

//4. Con trỏ hàm (Function Pointer)
//====================================================
/*
#include <stdint.h>

// Định nghĩa kiểu con trỏ hàm
typedef void (*callback_t)(void);

// Hàm ví dụ
void led_on(void) {
    // Bật LED (giả lập)
}

void led_off(void) {
    // Tắt LED (giả lập)
}

int main(void) {
    callback_t action; // Khai báo con trỏ hàm
    action = led_on;   // Gán hàm led_on cho con trỏ
    action();          // Gọi hàm thông qua con trỏ (bật LED)

    action = led_off;  // Gán hàm led_off
    action();          // Gọi hàm (tắt LED)
    return 0;
}
*/
//====================================================
//5. Quản lý bộ nhớ động (Dynamic Memory)
//====================================================
/*
#include <stdlib.h>

int main(void) {
    uint8_t *ptr = (uint8_t *)malloc(10 * sizeof(uint8_t)); // Cấp phát 10 byte
    if (ptr == NULL) {
        // Xử lý lỗi nếu cấp phát thất bại
        return -1;
    }

    for (uint8_t i = 0; i < 10; i++) {
        ptr[i] = i; // Gán giá trị cho bộ nhớ vừa cấp phát
    }

    free(ptr); // Giải phóng bộ nhớ
    return 0;
}
*/
//====================================================
