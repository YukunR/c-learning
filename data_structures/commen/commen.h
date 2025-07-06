/**
 * common.h - 通用定义和工具函数
 * 所有数据结构共用的基础功能
 */

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// ========== 通用比较函数 ==========

/**
 * 比较两个整数
 * 返回: a < b 时返回负数, a == b 时返回0, a > b 时返回正数
 */
int compare_int(const void *a, const void *b);

/**
 * 比较两个字符串
 */
int compare_string(const void *a, const void *b);

/**
 * 比较两个浮点数
 */
int compare_float(const void *a, const void *b);

/**
 * 比较两个双精度浮点数
 */
int compare_double(const void *a, const void *b);

// ========== 通用打印函数 ==========

/**
 * 打印整数
 */
void print_int(const void *data);

/**
 * 打印字符串
 */
void print_string(const void *data);

/**
 * 打印浮点数
 */
void print_float(const void *data);

// ========== 克隆函数 ==========
void *clone_int(const void *data);
void *clone_string(const void *data);
void *clone_float(const void *data);

// ========== 内存管理辅助 ==========

/**
 * 安全的malloc，失败时打印错误信息
 */
void *safe_malloc(size_t size);

/**
 * 安全的free，自动将指针设为NULL
 */
void safe_free(void **ptr);

// ========== 调试辅助 ==========

/**
 * 调试模式开关
 */
extern bool debug_mode;

/**
 * 调试输出宏
 */
#define DEBUG_PRINT(fmt, ...)                           \
    do                                                  \
    {                                                   \
        if (debug_mode)                                 \
        {                                               \
            printf("[DEBUG] " fmt "\n", ##__VA_ARGS__); \
        }                                               \
    } while (0)

/**
 * 错误输出宏
 */
#define ERROR_PRINT(fmt, ...)                                \
    do                                                       \
    {                                                        \
        fprintf(stderr, "[ERROR] " fmt "\n", ##__VA_ARGS__); \
    } while (0)

#endif