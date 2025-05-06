/* 
### 类型转换综合练习题：类型转换小助手

编写一个 C 程序，实现一个简易的“类型转换小助手”，练习 int、float、char 和字符串之间的类型转换。

### 输入部分：

1. 用户输入一个浮点数字符串（例如："3.14abc"）；
2. 用户输入一个整数（int）；
3. 用户输入一个单字符（如：'A'）。

### 处理部分（使用函数封装）：

1. 将输入的浮点数字符串转换为 float 并乘以 10，保留小数结果；
2. 将整数强制转换为 float，再转换为 char，并打印对应的字符；
3. 将输入字符转换为其对应的 ASCII 整数值；
4. 使用 sprintf() 拼接一个完整句子，格式如下：

```
你输入的浮点数为 3.14, 乘以 10 后为 31.40, 你输入的整数为 66, 对应的字符为 B, 你输入的字符为 Z, 对应的 ASCII 为 90。
```

### 示例运行：

```
请输入一个浮点数字符串：3.14abc
请输入一个整数：66
请输入一个字符：Z

--- 输出 ---
你输入的浮点数为 3.14, 乘以 10 后为 31.40, 你输入的整数为 66, 对应的字符为 B, 你输入的字符为 Z, 对应的 ASCII 为 90。
```

### 要求：

- 所有功能用函数封装；
- 使用 strtof() 安全转换字符串为浮点数；
- 使用 (char)、(int) 做强制转换；
- 使用 sprintf() 构造格式化字符串；
- 所有字符串变量使用 char 数组，不使用动态分配。

### 提示函数

- `float strtof(const char *nptr, char **endptr);`
- `char (int)` 强制转换为字符
- `int (char)` 获取字符 ASCII 值
- `sprintf(char *buf, const char *format, ...)` 格式化字符串
*/

#include <stdio.h>
#include <stdlib.h>

void handle_string(float *result1, float *result2);
void handle_int(int *result1, char *result2);
void handle_char(char *result1, int *result2);

int main() {
    char output[200];
    float f1, f2;
    int int_from_handle_int, int_from_handle_char;
    char c_from_handle_int, c_from_handle_char;
    handle_string(&f1, &f2);
    handle_int(&int_from_handle_int, &c_from_handle_int);
    handle_char(&c_from_handle_char, &int_from_handle_char);
    // 格式化输出
    printf("你输入的浮点数为 %.2f, 乘以 10 后为 %.2f, ", f1, f2);
    printf("你输入的整数为 %d, 对应的字符为 %c, ", int_from_handle_int, c_from_handle_int);
    printf("你输入的字符为 %c, 对应的 ASCII 为 %d。\n", c_from_handle_char, int_from_handle_char);
    return 0;
}

void handle_string(float *result1, float *result2) {
    char input_str[100];
    printf("请输入一个浮点数字符串: ");
    scanf("%s", input_str);

    char *end;
    float f = strtof(input_str, &end);

    *result1 = f;
    *result2 = f * 10;
}

void handle_int(int *result1, char *result2) {
    int input_int;
    printf("请输入一个整数 (0-127): ");
    scanf("%d", &input_int);
    // 转换为 float
    float f = (float)input_int;
    // 转换为 char
    char c = (char)input_int;

    *result1 = input_int;
    *result2 = c;
}

void handle_char(char *result1, int *result2) {
    char input_char;
    printf("请输入一个字符: ");
    scanf(" %c", &input_char);
    int ascii_char = (int)input_char;
    *result1 = input_char;
    *result2 = ascii_char;
}