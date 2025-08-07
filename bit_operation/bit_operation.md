# C 语言位运算完全指南

## 概述

位运算是直接对整数在内存中的二进制表示进行操作的运算。它是最接近硬件的运算方式，具有极高的执行效率。在数据结构、算法优化、系统编程中广泛应用。

## 基础概念

### 二进制表示

在学习位运算前，需要熟悉数字的二进制表示：

```
十进制    二进制      十六进制
0        00000000    0x00
1        00000001    0x01
2        00000010    0x02
3        00000011    0x03
4        00000100    0x04
5        00000101    0x05
...
15       00001111    0x0F
16       00010000    0x10
255      11111111    0xFF
```

### 有符号数的表示

**原码、反码、补码：**

```c
// 以8位为例
int8_t x = 5;    // 原码: 00000101
int8_t y = -5;   // 补码: 11111011

// 负数的补码 = 原码取反 (符号位除外) + 1
// -5的原码: 10000101
// 取反:     11111010
// 加1:      11111011 (这就是-5的补码)
```

**为什么使用补码？**

- 统一加法和减法运算
- 零的表示唯一
- 简化硬件设计

## 位运算操作符

### 1. 按位与（&）- AND

**规则：** 两个位都为 1 时结果为 1，否则为 0

```c
    1010 (10)
&   1100 (12)
-----------
    1000 (8)
```

**用途：**

- **清零特定位**：`x & 0` 清零所有位
- **保留特定位**：`x & mask` 只保留 mask 为 1 的位
- **检查奇偶性**：`x & 1` 检查最低位（奇数为 1，偶数为 0）

```c
// 示例：检查数字是否为偶数
bool is_even(int x) {
    return (x & 1) == 0;
}

// 示例：清除最低4位
int clear_low_4bits(int x) {
    return x & 0xFFFFFFF0;  // ...11110000
}
```

### 2. 按位或（|）- OR

**规则：** 两个位有一个为 1 时结果为 1，都为 0 时为 0

```c
    1010 (10)
|   1100 (12)
-----------
    1110 (14)
```

**用途：**

- **设置特定位**：`x | mask` 将 mask 为 1 的位设置为 1
- **合并标志**：组合多个布尔标志

```c
// 示例：设置第3位为1（从0开始计数）
int set_bit3(int x) {
    return x | (1 << 3);  // x | 00001000
}

// 示例：权限标志合并
#define READ_PERM    0x01  // 00000001
#define WRITE_PERM   0x02  // 00000010
#define EXEC_PERM    0x04  // 00000100

int full_permission = READ_PERM | WRITE_PERM | EXEC_PERM;  // 00000111
```

### 3. 按位异或（^）- XOR

**规则：** 两个位不同时结果为 1，相同时为 0

```c
    1010 (10)
^   1100 (12)
-----------
    0110 (6)
```

**特殊性质：**

- `x ^ 0 = x` （与 0 异或保持不变）
- `x ^ x = 0` （任何数与自己异或为 0）
- `x ^ y ^ y = x` （异或满足结合律，可以"抵消"）

**用途：**

- **翻转特定位**：`x ^ mask` 翻转 mask 为 1 的位
- **简单加密**：`encrypted = data ^ key`，`data = encrypted ^ key`
- **无临时变量交换**：交换两个变量的值

```c
// 示例：不用临时变量交换两个数
void swap_without_temp(int *a, int *b) {
    if (a != b) {  // 防止同一地址导致清零
        *a = *a ^ *b;
        *b = *a ^ *b;  // b = (a^b) ^ b = a
        *a = *a ^ *b;  // a = (a^b) ^ a = b
    }
}

// 示例：翻转第5位
int flip_bit5(int x) {
    return x ^ (1 << 5);
}
```

### 4. 按位取反（~）- NOT

**规则：** 0 变 1，1 变 0

```c
    ~1010 = 0101  // 对于4位数
    ~0000 = 1111
```

**注意：** C 语言中`~`对所有位取反，包括符号位

```c
// 8位示例
uint8_t x = 0x0F;  // 00001111
uint8_t y = ~x;    // 11110000 = 0xF0

// 32位示例（注意符号）
int x = 5;         // 00000000 00000000 00000000 00000101
int y = ~x;        // 11111111 11111111 11111111 11111010 = -6
```

### 5. 左移（<<）

**规则：** 所有位向左移动，右边补 0

```c
    0001 << 1 = 0010
    0001 << 2 = 0100
    0001 << 3 = 1000
```

**数学意义：** 左移 n 位等于乘以 2^n

```c
int x = 5;        // 101
int y = x << 2;   // 10100 = 20 = 5 * 2^2

// 常用：计算2的幂
int power_of_2(int n) {
    return 1 << n;  // 2^n
}
```

**注意事项：**

- 左移可能导致溢出
- 左移负数是未定义行为
- 移位数超过类型位数是未定义行为

### 6. 右移（>>）

**规则：** 所有位向右移动

```c
    1000 >> 1 = 0100
    1000 >> 2 = 0010
    1000 >> 3 = 0001
```

**两种类型：**

- **逻辑右移**：左边补 0
- **算术右移**：左边补符号位（有符号数）

```c
// 无符号数：总是逻辑右移
uint8_t x = 0x80;  // 10000000
uint8_t y = x >> 1; // 01000000 = 0x40

// 有符号数：通常是算术右移
int8_t a = -8;      // 11111000
int8_t b = a >> 1;  // 11111100 = -4（符号位扩展）
```

## 常用位运算技巧

### 1. 位操作基本技巧

```c
// 检查第n位是否为1
bool check_bit(int x, int n) {
    return (x & (1 << n)) != 0;
}

// 设置第n位为1
int set_bit(int x, int n) {
    return x | (1 << n);
}

// 清除第n位（设为0）
int clear_bit(int x, int n) {
    return x & ~(1 << n);
}

// 翻转第n位
int flip_bit(int x, int n) {
    return x ^ (1 << n);
}

// 获取最低n位
int get_low_n_bits(int x, int n) {
    return x & ((1 << n) - 1);
}
```

### 2. 数学运算优化

```c
// 判断是否为2的幂
bool is_power_of_2(int x) {
    return x > 0 && (x & (x - 1)) == 0;
}
/* 原理：2的幂只有一个1位
   x     = 1000
   x-1   = 0111
   x&(x-1) = 0000
*/

// 计算绝对值（仅限32位int）
int abs_bitwise(int x) {
    int mask = x >> 31;  // 符号位扩展：负数为-1，正数为0
    return (x + mask) ^ mask;
}

// 快速除以2^n
int divide_by_power_of_2(int x, int n) {
    return x >> n;  // 仅适用于正数或确定的负数处理
}

// 快速乘以2^n
int multiply_by_power_of_2(int x, int n) {
    return x << n;
}
```

### 3. 位计数技巧

```c
// 计算二进制中1的个数（Brian Kernighan算法）
int count_set_bits(int x) {
    int count = 0;
    while (x) {
        x &= (x - 1);  // 清除最右边的1
        count++;
    }
    return count;
}
/* 原理：x & (x-1) 会清除x最右边的1
   例如：x = 1100, x-1 = 1011, x & (x-1) = 1000
*/

// 找到最右边的1的位置
int rightmost_set_bit(int x) {
    return x & (-x);
}
/* 原理：-x 是 x 的补码，等于 ~x + 1
   x  = 1100  (12)
   ~x = 0011  (3)
   -x = 0100  (4)
   x & (-x) = 0100 (只保留最右边的1)
*/
```

### 4. 实用工具函数

```c
// 交换两个数的特定位
int swap_bits(int x, int i, int j) {
    // 检查第i位和第j位是否不同
    if (((x >> i) & 1) != ((x >> j) & 1)) {
        // 如果不同，则翻转这两位
        x ^= (1 << i) | (1 << j);
    }
    return x;
}

// 反转二进制位
uint32_t reverse_bits(uint32_t x) {
    uint32_t result = 0;
    for (int i = 0; i < 32; i++) {
        result = (result << 1) | (x & 1);
        x >>= 1;
    }
    return result;
}

// 循环左移
uint32_t rotate_left(uint32_t x, int n) {
    n %= 32;  // 确保n在有效范围内
    return (x << n) | (x >> (32 - n));
}
```

## 位运算在哈希函数中的应用

### 1. 位混合技术

```c
// Thomas Wang的32位整数哈希
uint32_t hash_int32(uint32_t key) {
    key = (key ^ 61) ^ (key >> 16);    // 异或高低位
    key = key + (key << 3);            // 左移并相加
    key = key ^ (key >> 4);            // 再次异或
    key = key * 0x27d4eb2d;            // 乘以质数
    key = key ^ (key >> 15);           // 最终异或
    return key;
}
```

### 2. 快速模运算优化

```c
// 当表大小是2的幂时，可以用位运算替代模运算
size_t hash_to_index_fast(size_t hash, size_t table_size) {
    // 要求：table_size 必须是2的幂
    return hash & (table_size - 1);
}
// 原理：对于2^n，x % (2^n) == x & (2^n - 1)
```

## 调试和可视化工具

```c
// 打印数字的二进制表示
void print_binary(unsigned int n, int bits) {
    for (int i = bits - 1; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i % 4 == 0 && i != 0) printf(" ");  // 每4位添加空格
    }
    printf("\n");
}

// 使用示例
int main() {
    int x = 42;
    printf("数字 %d 的二进制表示: ", x);
    print_binary(x, 8);

    printf("设置第1位后: ");
    print_binary(set_bit(x, 1), 8);

    return 0;
}
```

## 常见陷阱和注意事项

### 1. 优先级陷阱

```c
// 错误：位运算优先级低于比较运算符
if (x & 1 == 0)  // 实际执行：x & (1 == 0) = x & 0 = 0

// 正确：使用括号
if ((x & 1) == 0)
```

### 2. 符号扩展问题

```c
// 小心有符号数的右移
int8_t x = -1;  // 11111111
int y = x >> 4; // 可能是 11111111（符号扩展）而不是 00001111
```

### 3. 移位数量检查

```c
// 危险：移位数量超过类型位数
int x = 1;
int y = x << 32;  // 未定义行为（对于32位int）

// 安全：检查移位数量
int safe_left_shift(int x, int n) {
    if (n < 0 || n >= 32) return 0;  // 或者其他错误处理
    return x << n;
}
```

## 性能考虑

位运算的优势：

- **速度快**：直接对应 CPU 指令
- **节省内存**：紧凑的数据表示
- **无副作用**：不涉及函数调用

性能对比示例：

```c
// 慢：使用除法和模运算
bool is_even_slow(int x) { return x % 2 == 0; }
int divide_by_8_slow(int x) { return x / 8; }

// 快：使用位运算
bool is_even_fast(int x) { return (x & 1) == 0; }
int divide_by_8_fast(int x) { return x >> 3; }
```

## 实际应用示例

### 1. 权限系统

```c
#define PERM_READ    0x01  // 001
#define PERM_WRITE   0x02  // 010
#define PERM_EXECUTE 0x04  // 100

// 设置权限
int permissions = PERM_READ | PERM_WRITE;

// 检查权限
bool can_read(int perms) { return (perms & PERM_READ) != 0; }
bool can_write(int perms) { return (perms & PERM_WRITE) != 0; }

// 添加权限
permissions |= PERM_EXECUTE;

// 移除权限
permissions &= ~PERM_WRITE;
```

### 2. 状态标志

```c
typedef enum {
    FLAG_DIRTY    = 1 << 0,  // 0001
    FLAG_VISIBLE  = 1 << 1,  // 0010
    FLAG_SELECTED = 1 << 2,  // 0100
    FLAG_LOCKED   = 1 << 3   // 1000
} ObjectFlags;

void set_flag(int *flags, ObjectFlags flag) {
    *flags |= flag;
}

void clear_flag(int *flags, ObjectFlags flag) {
    *flags &= ~flag;
}

bool has_flag(int flags, ObjectFlags flag) {
    return (flags & flag) != 0;
}
```

这个指南涵盖了位运算的所有基础知识和实用技巧。掌握这些内容后，你就能够理解哈希函数中的各种位操作，并在自己的程序中有效使用位运算进行优化。
