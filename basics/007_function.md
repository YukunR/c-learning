# 函数的定义、声明与调用

## 函数的基本定义与调用

```c
#include <stdio.h>

void say_hello() {
    printf("Hello, world!\n");
}

int main() {
    say_hello();  // 调用
    return 0;
}
```

## 有返回值的函数

```c
#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int main() {
    int result = add(3, 5);
    printf("结果为 %d\n", result);
    return 0;
}
```

## 模块化编程，分离声明和调用

声明调用分开的目的在于，可以在头文件中声明并暴露接口

```c
#include <stdio.h>

int square(int x);  // 写在 main 前

int main() {
    printf("结果为 %d\n", square(4));
    return 0;
}

int square(int x) {
    return x * x;
}
```

## 形参、实参的传值关系

C 中函数参数默认是**值传递**，不是引用传递（除非用指针）

```c
#include <stdio.h>

void change(int x) {
    x = 100;
}

int main() {
    int a = 10;
    change(a);
    printf("a = %d\n", a);  // 输出仍为 10
    return 0;
}
```

## 没有返回值的函数，函数的函数

```c
#include <stdio.h>

int square(int x) {
    return x * x;
}

// 接收一个函数指针作为参数
void apply_and_print(int (*func)(int), int value) {
    printf("结果是：%d\n", func(value));
}

int main() {
    apply_and_print(square, 5);  // 把函数传进去
    return 0;
}
```

## 练习 (来自 ChatGPT)

### 题目

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
