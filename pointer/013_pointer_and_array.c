#include <stdio.h>

int main()
{
    int arr[5];
    int *p = arr;
    printf("请输入5个数字:\n");

    for (int i = 0; i < 5; i++)
    {
        int n;
        printf("请输入第 %d 个数字:", i + 1);
        scanf("%d", &n);
        *(p + i) = n;
    }

    // 计算平均值
    int sum = 0;
    p = arr;
    for (int i = 0; i < 5; i++)
    {
        sum += *(p + i);
    }

    float ave = (float)sum / 5.0;
    printf("平均值为: %.2f\n", ave);

    // 逆序输出
    p = arr + 4;
    for (int i = 4; i >= 0; i--)
    {
        printf("%d\n", *(p - i));
    }

    return 0;
}