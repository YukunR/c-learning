# C 语言：类型声明与保留字

## 变量声明格式

```c
<类型> <变量名> = <初始值>;
```

示例：

```c
int age = 25;
char grade = 'A';
float weight = 56.7f;
```

多个变量也可同时声明：

```c
int a = 1, b = 2, c = 3;
```

---

## 关键字

C 语言总共有 32 个保留关键字（C89/C90），它们**不能用作变量名或函数名**：

```
auto     break     case      char     const
continue default   do        double   else
enum     extern    float     for      goto
if       int       long      register return
short    signed    sizeof    static   struct
switch   typedef   union     unsigned void
volatile while
```

分类如下：

1. 基本数据类型 (5 个):
   | 类型 | 含义 | 示例 |
   |----------|------|------|
   | `void` | 表示无类型，用于函数无返回值、无参数，或表示通用指针类型 | `void printHello(void);`<br>`void* ptr = NULL;` |
   | `char` | 字符类型，本质上是整型的一种，1 字节 | `char c = 'A';` |
   | `int` | 整型，大小通常与平台有关（一般为 4 字节） | `int age = 18;` |
   | `float` | 单精度浮点数，4 字节 | `float pi = 3.14f;` |
   | `double` | 双精度浮点数，8 字节 | `double pi = 3.14159;` |
2. 类型修饰关键字 (4 个)：
   | 关键字 | 含义 | 示例 |
   |--------|------|------|
   | `short` | 修饰 `int`，表示短整型（通常 2 字节） | `short s = 100;` |
   | `long` | 修饰 `int`，表示长整型（通常 8 字节） | `long l = 1000000L;` |
   | `signed` | 有符号整型（默认） | `signed int x = -5;` |
   | `unsigned` | 无符号整型 | `unsigned int y = 10;` |
3. 复杂类型关键字 (5 个)：
   | 关键字 | 用途 | 示例 |
   |------------|------|------|
   | `struct` | 结构体类型声明 | `struct Point { int x, y; };` |
   | `union` | 共用体声明（共享内存） | `union Data { int i; float f; };` |
   | `enum` | 枚举类型声明 | `enum Color { RED, GREEN, BLUE };` |
   | `typedef` | 给类型起别名 | `typedef unsigned int uint;` |
   | `sizeof` | 获取类型或变量的字节大小 | `printf("%zu", sizeof(int));` |
4. 储存类型关键字 (6 个)：
   | 关键字 | 含义 | 示例 |
   |------------|------|------|
   | `auto` | 自动存储（局部变量默认的存储类型） | `auto int a = 10;`（可省略） |
   | `static` | 静态变量或函数，只初始化一次，跨函数调用保持值 | `static int count = 0;` |
   | `register` | 寄存器变量（建议存于 CPU 寄存器） | `register int i;` |
   | `extern` | 外部变量声明（跨文件使用） | `extern int sharedValue;` |
   | `const` | 常量修饰符，不可修改 | `const int max = 100;` |
   | `volatile` | 指明变量可能被系统或中断修改 | `volatile int flag;` |
5. 流程控制等 (12 个)：
   | 关键字 | 含义 | 示例 |
   |------------|------|------|
   | `if` | 条件判断 | `if (x > 0) {...}` |
   | `else` | 条件为假执行的分支 | `else {...}` |
   | `switch` | 多分支结构 | `switch (option) { case 1: ... }` |
   | `case` | `switch` 分支标签 | `case 1: printf("选项1");` |
   | `default` | `switch` 默认分支 | `default: printf("其他");` |
   | `return` | 从函数返回值 | `return 0;` |
   | `break` | 结束循环或 `switch` | `if (x > 10) break;` |
   | `continue` | 跳过本次循环，进行下一次 | `if (x == 0) continue;` |
   | `goto` | 无条件跳转 | `goto end;`（不推荐） |
   | `if` | 条件判断 | `if (x > 0) {...}` |
   | `else` | 条件为假执行的分支 | `else {...}` |
   | `switch` | 多分支结构 | `switch (option) { case 1: ... }` |
   | `case` | `switch` 分支标签 | `case 1: printf("选项1");` |
   | `default` | `switch` 默认分支 | `default: printf("其他");` |

说明：C 没有原生布尔类型，但是可以：
```c
#include <stdbool.h>
bool flag = true;
```