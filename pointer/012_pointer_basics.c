#include <stdio.h>


int main() {
    int x = 1;
    int *p = &x;
    *p = 3;
    printf("%d\n", *p);

    // 修改多个值
    int y = 2;
    int z = 3;
    p = &y;
    *p = 1;
    p = &z;
    *p = 1;
    printf("%d, %d\n", y, z);

    // 打印指针地址
    printf("%p\n", (void*)&p);
    return 0;
}
