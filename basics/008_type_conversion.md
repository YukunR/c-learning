# C 语言类型转换笔记

## 类型转换分类

C 语言中类型转换分为两类：

| 类型                 | 说明             | 示例                    |
| -------------------- | ---------------- | ----------------------- |
| 隐式转换             | 编译器自动完成   | float f = 1 + 2.5;      |
| 显式转换（强制转换） | 手动指定目标类型 | float f = (float)3 / 2; |

## 强制类型转换语法

```c
(new_type)(expression)
```

示例：

```c
int a = 5;
int b = 2;
float result = (float)a / b;  // 输出 2.5，而非 2
```

## 常见类型转换示例

| 从类型      | 转为类型       | 示例            |
| ----------- | -------------- | --------------- |
| int → float | (float)5 → 5.0 | 保留小数部分    |
| float → int | (int)5.7 → 5   | 小数被截断      |
| char → int  | 'A' → 65       | 字符的 ASCII 值 |
| int → char  | (char)65 → 'A' | 强制转字符      |

## 整数除法注意事项

```c
int a = 5;
int b = 2;
float r = a / b;           // 结果是 2
float correct = (float)a / b;  // 结果是 2.5
```

## 字符串与数字之间的转换

### 1. 整数 → 字符串（使用 sprintf()）

```c
#include <stdio.h>

int number = 123;
char str[20];
sprintf(str, "%d", number);  // str = "123"
```

说明：

- 把格式化的输出内容写入字符串中；
- 类似于 Python 的 str(123) 或 f"{123}"。

### 2. 字符串 → 整数（使用 atoi()）

```c
#include <stdlib.h>

char str[] = "456";
int num = atoi(str);  // num = 456
```

注意：

- 不安全：atoi("abc") → 0，无报错；
- 推荐替代函数见下方。

### 3. 字符串 → 整数（更安全：使用 strtol()）

```c
#include <stdlib.h>

char *end;
long num = strtol("123abc", &end, 10);  // num = 123, *end = "abc"
```

优点：

- 可检测非法输入；
- 可选择进制（如十进制 10、十六进制 16）；
- strtol 适用于 long，strtoul 适用于 unsigned long。

### 4. 字符串 → 浮点数（使用 atof()）

```c
#include <stdlib.h>

char str[] = "3.14";
float f = atof(str); // f = 3.14
```

注意：

- atof("3.14abc") → 3.14（忽略非数字部分）；
- 不推荐用于输入校验；

### 5. 字符串 → 浮点数（推荐：使用 strtof() 或 strtod()）

```c
#include <stdlib.h>

char *end;
float f = strtof("2.71abc", &end); // f = 2.71, *end = "abc"
```

| 函数名   | 返回类型 | 推荐使用 |
| -------- | -------- | -------- |
| strtof() | float    | ✅       |
| strtod() | double   | ✅       |

## 类型转换规则总结

| 原始类型    | 目标类型           | 是否自动转换    |
| ----------- | ------------------ | --------------- |
| int → float | ✅ 自动            | ❌              |
| float → int | ❌ 手动 (int)      | ✅ 截断小数     |
| long → int  | ❌ 手动 (int)      | ✅ 如果超出范围 |
| int → char  | ✅ 自动（低 8 位） | ✅ 可能丢失信息 |
| char → int  | ✅ 自动提升        | ❌              |

## 练习 (来自 ChatGPT)

### 类型转换综合练习题：类型转换小助手

编写一个 C 程序，实现一个简易的“类型转换小助手”，练习 int、float、char 和字符串之间的类型转换。

### 输入部分：

1. 用户输入一个浮点数字符串（例如："3.14abc"）；
2. 用户输入一个整数（int）；
3. 用户输入一个单字符（如：'A'）。

### 处理部分（使用函数封装）：

1. 将输入的浮点数字符串转换为 float 并乘以 10，保留小数结果；
2. 将整数强制转换为 float，再转换为 char，并打印 ASCII 字符；
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
- 使用 `strtof()` 安全转换字符串为浮点数；
- 使用 `(char)`、`(int)` 做强制转换；
- 使用 `sprintf()` 构造格式化字符串；
- 所有字符串变量使用 char 数组，不使用动态分配。

### 提示函数

- `float strtof(const char *nptr, char **endptr);`
- `char (int)` 强制转换为字符
- `int (char)` 获取字符 ASCII 值
- `sprintf(char *buf, const char *format, ...)` 格式化字符串

不用指针，使用结构体也可完成这个练习。
