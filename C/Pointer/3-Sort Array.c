#include <stdio.h>
#include <stdlib.h>

void sortArray(int *arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (*(arr + i) > *(arr + j)) {
                int temp = *(arr + i);
                *(arr + i) = *(arr + j);
                *(arr + j) = temp;
            }
        }
    }
}

int main() {
    int size;
    printf("Enter size of array: ");
    scanf("%d", &size);

    // Cấp phát động bộ nhớ
    int *arr = (int *)malloc(size * sizeof(int));
    if (arr == NULL) {
        printf("Not enough memory!\n");
        return 1;
    }

    // Nhập mảng
    printf("Enter %d element(s):\n", size);
    for (int i = 0; i < size; i++) {
        scanf("%d", arr + i);
    }

    // Sắp xếp mảng
    sortArray(arr, size);

    // In mảng đã sắp xếp
    printf("Sorted array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", *(arr + i));
    }
    printf("\n");

    // Giải phóng bộ nhớ
    free(arr);
    return 0;
}
