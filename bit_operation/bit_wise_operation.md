# 位运算综合练习 - 位操作工具库

## 练习概述

你需要实现一个位操作工具库，包含一系列实用的位运算函数。这个练习将帮助你巩固所有基础位运算操作，并学会如何将它们应用到实际问题中。

## 项目结构

```
bitwise_utils/
├── bitwise_utils.h    // 头文件声明
├── bitwise_utils.c    // 实现文件
└── test_bitwise.c     // 测试程序
```

## 任务列表

### 第一部分：基础位操作函数（难度：⭐⭐）

实现以下基础位操作函数：

```c
// bitwise_utils.h
#ifndef BITWISE_UTILS_H
#define BITWISE_UTILS_H

#include <stdint.h>
#include <stdbool.h>

// 1. 检查第n位是否为1（从0开始计数）
bool bit_is_set(uint32_t value, int n);

// 2. 设置第n位为1
uint32_t bit_set(uint32_t value, int n);

// 3. 清除第n位（设为0）
uint32_t bit_clear(uint32_t value, int n);

// 4. 翻转第n位
uint32_t bit_flip(uint32_t value, int n);

// 5. 获取最低n位的值
uint32_t get_low_bits(uint32_t value, int n);

#endif
```

**测试用例：**

```c
// 测试示例
uint32_t x = 0b10110100;  // 180

printf("第2位是否为1: %s\n", bit_is_set(x, 2) ? "是" : "否");     // 应输出"是"
printf("设置第0位: %u\n", bit_set(x, 0));                        // 应输出181
printf("清除第7位: %u\n", bit_clear(x, 7));                      // 应输出52
printf("翻转第1位: %u\n", bit_flip(x, 1));                       // 应输出182
printf("最低4位: %u\n", get_low_bits(x, 4));                     // 应输出4
```

### 第二部分：计数和查找函数（难度：⭐⭐⭐）

实现更高级的位操作函数：

```c
// 6. 计算二进制中1的个数（使用Brian Kernighan算法）
int count_set_bits(uint32_t value);

// 7. 计算二进制中0的个数
int count_clear_bits(uint32_t value);

// 8. 找到最右边的1的位置（返回位索引，如果没有1则返回-1）
int find_rightmost_set_bit(uint32_t value);

// 9. 找到最左边的1的位置（返回位索引，如果没有1则返回-1）
int find_leftmost_set_bit(uint32_t value);

// 10. 检查一个数是否是2的幂
bool is_power_of_2(uint32_t value);
```

**测试用例：**

```c
uint32_t x = 0b10110100;  // 180

printf("1的个数: %d\n", count_set_bits(x));              // 应输出4
printf("0的个数: %d\n", count_clear_bits(x));            // 应输出28（假设32位）
printf("最右边1的位置: %d\n", find_rightmost_set_bit(x)); // 应输出2
printf("最左边1的位置: %d\n", find_leftmost_set_bit(x));  // 应输出7
printf("是否为2的幂: %s\n", is_power_of_2(16) ? "是" : "否"); // 应输出"是"
```

### 第三部分：位模式操作（难度：⭐⭐⭐⭐）

实现复杂的位模式操作：

```c
// 11. 反转所有位（32位）
uint32_t reverse_bits(uint32_t value);

// 12. 循环左移n位
uint32_t rotate_left(uint32_t value, int n);

// 13. 循环右移n位
uint32_t rotate_right(uint32_t value, int n);

// 14. 交换两个位的值
uint32_t swap_bits(uint32_t value, int i, int j);

// 15. 提取位域（从position开始的length位）
uint32_t extract_bits(uint32_t value, int position, int length);
```

**测试用例：**

```c
uint32_t x = 0b10110100;  // 180

printf("反转位: 0x%08X\n", reverse_bits(x));
printf("左移3位: %u\n", rotate_left(x, 3));
printf("右移3位: %u\n", rotate_right(x, 3));
printf("交换第1位和第5位: %u\n", swap_bits(x, 1, 5));
printf("提取第2-5位: %u\n", extract_bits(x, 2, 4));
```

### 第四部分：实用工具函数（难度：⭐⭐⭐）

实现一些实用的工具函数：

```c
// 16. 打印32位数的二进制表示（格式化输出，每4位一组）
void print_binary(unsigned int n, int bits);

// 17. 计算两个数的汉明距离（不同位的个数）
int hamming_distance(uint32_t a, uint32_t b);

// 18. 检查奇偶性
bool is_even(uint32_t value);

// 19. 无临时变量交换两个数
void swap_without_temp(uint32_t *a, uint32_t *b);

// 20. 快速计算 value * (2^n)
uint32_t multiply_by_power_of_2(uint32_t value, int n);
```

### 第五部分：位标志管理系统（难度：⭐⭐⭐⭐⭐）

实现一个完整的位标志管理系统：

```c
// 位标志定义
typedef enum {
    FLAG_NONE     = 0,
    FLAG_ACTIVE   = 1 << 0,   // 0001
    FLAG_VISIBLE  = 1 << 1,   // 0010
    FLAG_DIRTY    = 1 << 2,   // 0100
    FLAG_LOCKED   = 1 << 3,   // 1000
    FLAG_SELECTED = 1 << 4,   // 10000
    FLAG_ALL      = 0x1F      // 11111
} ObjectFlags;

// 位标志管理函数
typedef struct {
    uint32_t flags;
} FlagManager;

// 21. 创建标志管理器
FlagManager* flag_manager_create(void);

// 22. 销毁标志管理器
void flag_manager_destroy(FlagManager **manager);

// 23. 设置单个标志
void flag_set(FlagManager *manager, ObjectFlags flag);

// 24. 清除单个标志
void flag_clear(FlagManager *manager, ObjectFlags flag);

// 25. 检查标志是否设置
bool flag_is_set(FlagManager *manager, ObjectFlags flag);

// 26. 切换标志状态
void flag_toggle(FlagManager *manager, ObjectFlags flag);

// 27. 设置多个标志
void flag_set_multiple(FlagManager *manager, uint32_t flags);

// 28. 清除所有标志
void flag_clear_all(FlagManager *manager);

// 29. 获取所有标志的字符串表示
char* flag_to_string(FlagManager *manager);

// 30. 比较两个标志管理器
bool flag_equals(FlagManager *a, FlagManager *b);
```

## 挑战任务

### 挑战 1：位压缩存储（⭐⭐⭐⭐⭐）

实现一个位压缩数组，可以存储任意位宽的整数：

```c
// 例如：存储1000个3位数字，只需要 (1000*3+7)/8 = 375 字节
// 而不是 1000*4 = 4000 字节

typedef struct {
    uint8_t *data;
    size_t capacity;  // 总位数
    int bits_per_item; // 每个元素的位数
} BitArray;

BitArray* bitarray_create(size_t count, int bits_per_item);
void bitarray_destroy(BitArray **array);
void bitarray_set(BitArray *array, size_t index, uint32_t value);
uint32_t bitarray_get(BitArray *array, size_t index);
```

### 挑战 2：位运算实现简单加密（⭐⭐⭐⭐）

实现一个基于异或的简单加密算法：

```c
// XOR密码：每个字节都与密钥进行异或
void xor_encrypt_decrypt(uint8_t *data, size_t length, const char *key);

// 测试代码
char message[] = "Hello, Bitwise World!";
char key[] = "mykey";
xor_encrypt_decrypt((uint8_t*)message, strlen(message), key);
printf("加密后: %s\n", message);
xor_encrypt_decrypt((uint8_t*)message, strlen(message), key);
printf("解密后: %s\n", message);  // 应该恢复原文
```

## 完整测试程序框架

```c
// test_bitwise.c
#include <stdio.h>
#include <assert.h>
#include "bitwise_utils.h"

void test_basic_operations(void) {
    printf("=== 测试基础位操作 ===\n");

    uint32_t x = 0b10110100;  // 180

    // 在这里添加你的测试代码
    assert(bit_is_set(x, 2) == true);
    assert(bit_set(x, 0) == 181);
    // ... 更多测试

    printf("基础位操作测试通过！\n\n");
}

void test_counting_functions(void) {
    printf("=== 测试计数函数 ===\n");
    // 你的测试代码
    printf("计数函数测试通过！\n\n");
}

void test_pattern_operations(void) {
    printf("=== 测试位模式操作 ===\n");
    // 你的测试代码
    printf("位模式操作测试通过！\n\n");
}

void test_utility_functions(void) {
    printf("=== 测试工具函数 ===\n");
    // 你的测试代码
    printf("工具函数测试通过！\n\n");
}

void test_flag_manager(void) {
    printf("=== 测试标志管理器 ===\n");
    // 你的测试代码
    printf("标志管理器测试通过！\n\n");
}

int main(void) {
    printf("开始位运算综合测试...\n\n");

    test_basic_operations();
    test_counting_functions();
    test_pattern_operations();
    test_utility_functions();
    test_flag_manager();

    printf("所有测试通过！🎉\n");
    return 0;
}
```

## 提示和建议

### 实现提示

1. **从简单开始**：按顺序实现，每完成一个函数就测试
2. **使用调试工具**：实现`print_binary`函数帮助调试
3. **注意边界条件**：检查 n 的范围，处理 value 为 0 的情况
4. **参考资料**：遇到困难时回顾位运算指南的相关部分

### 常见陷阱

1. **移位数量检查**：确保移位数量不超过类型位数
2. **优先级问题**：位运算优先级低于比较运算，记得加括号
3. **符号问题**：使用无符号类型避免符号扩展问题

### 扩展思考

完成基础任务后，思考以下问题：

1. 如何优化这些函数的性能？
2. 如何处理 64 位数字？
3. 如何实现线程安全的标志管理器？
4. 位运算在哈希函数中还有哪些应用？

## 评估标准

- **正确性**：所有测试用例通过
- **代码质量**：命名清晰，注释充分
- **效率**：使用最优的位运算实现
- **完整性**：实现所有要求的函数
- **创新性**：在挑战任务中展现创造力

祝你练习愉快！通过这个综合练习，你将彻底掌握位运算的精髓。💪
