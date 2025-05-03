/* 
编写一个数字处理小程序，要求用户输入一个整数，然后通过函数调用，完成以下操作：

请输入一个整数：[用户输入，例如：12345]

实现五个输出：

1. 判断是否为质数
2. 计算位数
3. 计算各位数字之和
4. 反转数字
5. 判断是否为回文数

提示：

- 所有功能都封装为函数；
- 不涉及指针，全部用值传递和返回值；
- 可用 n % 10 和 n / 10 拆分数字；
- 可使用 reverse(n) 和原数比较判断是否为回文数。
*/

#include <stdio.h>
#include <math.h>

int is_prime(int n);
int count_digits(int n);
int sum_digits(int n);
int reverse(int n);
int is_palindrome(int n);

int main() {
    int input_number;
    printf("请输入一个整数: ");
    scanf("%d", &input_number);

    printf("是否为质数：%s\n", is_prime(input_number) ? "是" : "否");
    printf("共有 %d 位\n", count_digits(input_number));
    printf("各位相加和为：%d\n", sum_digits(input_number));
    printf("反转数字：%d\n", reverse(input_number));
    printf("是否为回文数：%s\n", is_palindrome(input_number) ? "是" : "否");
    
    return 0;
}

int is_prime(int n) {
    if (n < 1) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

int count_digits(int n) {
    int digits = 0;
    n = abs(n);

    while (n > 0) {
        n /= 10;
        digits += 1;
    }
    return digits;
}

int sum_digits(int n) {
    int sum = 0;
    n = abs(n);
    
    while (n > 0) {
        int digit = n % 10;
        n /= 10;
        sum += digit;
    }
    
    return sum;
}

int reverse(int n) {
    int rev = 0;
    while (n > 0) {
        int digit = n % 10;
        rev = rev * 10 + digit;
        n /= 10;
    }
    return rev;
}

int is_palindrome(int n) {
    if (n < 0) return 0;
    return n == reverse(n);
}