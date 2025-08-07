// bitwise_utils.h
#ifndef BITWISE_UTILS_H
#define BITWISE_UTILS_H

#include <stdint.h>
#include <stdbool.h>

// basic operation
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

// count and get
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

// bit mode operation
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

// 16. print
void print_binary(unsigned int n, int bits);

// 17. 计算两个数的汉明距离（不同位的个数）
int hamming_distance(uint32_t a, uint32_t b);

// 18. 检查奇偶性
bool is_even(uint32_t value);

// 19. 无临时变量交换两个数
void swap_without_temp(uint32_t *a, uint32_t *b);

// 20. 快速计算 value * (2^n)
uint32_t multiply_by_power_of_2(uint32_t value, int n);

// flag manager
// 位标志定义
typedef enum
{
    FLAG_NONE = 0,
    FLAG_ACTIVE = 1 << 0,   // 00001
    FLAG_VISIBLE = 1 << 1,  // 00010
    FLAG_DIRTY = 1 << 2,    // 00100
    FLAG_LOCKED = 1 << 3,   // 01000
    FLAG_SELECTED = 1 << 4, // 10000
    FLAG_ALL = 0x1F         // 11111
} ObjectFlags;

// 位标志管理函数
typedef struct
{
    uint32_t flags;
} FlagManager;

// 21. 创建标志管理器
FlagManager *flag_manager_create(void);

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
char *flag_to_string(FlagManager *manager);

// 30. 比较两个标志管理器
bool flag_equals(FlagManager *a, FlagManager *b);

// 位压缩
typedef struct {
    uint8_t *data;
    size_t capacity;  // 总位数
    int bits_per_item; // 每个元素的位数
} BitArray;

BitArray *bitarray_create(size_t count, int bits_per_item);
void bitarray_destroy(BitArray **array);
void bitarray_set(BitArray *array, size_t index, uint32_t value);
uint32_t bitarray_get(BitArray *array, size_t index);

// 加密
void xor_encrypt_decrypt(uint8_t *data, size_t length, const char *key);

#endif