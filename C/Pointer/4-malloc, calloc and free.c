#include <stdio.h>
#include <stdlib.h>

int main() {
    //malloc()
    //====================================================
    int *ptr;
    int n = 5;

    // Cấp phát bộ nhớ cho mảng 5 phần tử kiểu int
    ptr = (int *)malloc(n * sizeof(int));

    if (ptr == NULL) {
        printf("Failed!\n");
        return 1;
    }

    // Gán giá trị cho mảng
    for (int i = 0; i < n; i++) {
        ptr[i] = i + 1;  // Gán 1, 2, 3, 4, 5
    }

    // In mảng
    printf("Array malloc: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", ptr[i]);
    }
    printf("\n");

    // Giải phóng bộ nhớ
    free(ptr);
    printf("Free malloc()\n\n");
    //====================================================

    //calloc()
    //====================================================
    ptr = (int *)calloc(n, sizeof(int));

    if (ptr == NULL) {
        printf("Failed!\n");
        return 1;
    }

    // In mảng (tất cả đều là 0)
    printf("Array calloc after allocation: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", ptr[i]);
    }
    printf("\n");

    // Gán giá trị
    for (int i = 0; i < n; i++) {
        ptr[i] = i + 1;
    }

    // In lại mảng
    printf("Array calloc after assign elements: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", ptr[i]);
    }
    printf("\n");

    // Giải phóng bộ nhớ
    free(ptr);
    printf("Free calloc()\n");
    //====================================================

    return 0;
}
