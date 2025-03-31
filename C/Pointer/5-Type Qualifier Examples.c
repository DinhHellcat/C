#include <stdio.h>

//vì là ví dụ nên sẽ không có khả năng chạy thử terminal :v
int main() {
    //const
    //====================================================
    const int x = 10; // x là hằng số, không thể thay đổi
    // x = 20; // Lỗi: không thể gán giá trị mới cho biến const
    printf("Giá trị của x: %d\n", x);
    return 0;

    //ứng dụng: định nghĩa hằng số hoặc bảo vệ tham số hàm khỏi bị thay đổi
    /*void printNumber(const int* ptr)
    {
        // *ptr = 50; // Lỗi: không thể thay đổi giá trị qua con trỏ const
        printf("Số: %d\n", *ptr);
    }*/
    //====================================================
    //volatile
    //====================================================
    volatile int sensor = 0; // Giả sử đây là giá trị từ cảm biến phần cứng
    while (sensor == 0) {
        // Vòng lặp chờ giá trị sensor thay đổi từ phần cứng
        printf("Đang chờ...\n");
    }
    printf("Sensor đã thay đổi: %d\n", sensor);
    return 0;

    //Nếu không có volatile, trình biên dịch có thể tối ưu hóa vòng lặp while bằng cách giả định sensor không đổi và gây ra lỗi logic.
    //Ứng dụng: Thường dùng trong lập trình nhúng (embedded) khi làm việc với thanh ghi phần cứng hoặc biến được chia sẻ giữa các thread.
    //====================================================
}
//====================================================
//restrict
/*
#include <stdio.h>

void copyArray(int* restrict dest, int* restrict src, int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i]; // Trình biên dịch biết dest và src không overlap
    }
}

int main() {
    int a[5] = {1, 2, 3, 4, 5};
    int b[5] = {0};
    copyArray(b, a, 5);
    for (int i = 0; i < 5; i++) {
        printf("%d ", b[i]);
    }
    return 0;

    //Nếu không có restrict, trình biên dịch phải giả định rằng dest và src có thể trỏ đến cùng vùng bộ nhớ (aliasing), dẫn đến việc sinh mã kém hiệu quả hơn.
    //Ứng dụng: Thường dùng trong các hàm xử lý mảng hoặc tối ưu hóa hiệu suất tính toán.
}*/
//====================================================
