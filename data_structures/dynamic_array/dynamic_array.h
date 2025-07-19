#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/*
 * 动态数组
 * 
 * 设计目标：
 * 1. 与双向循环链表保持接口风格一致
 * 2. 理解动态内存管理和扩容机制  
 * 3. 为后续学习栈、队列、堆打下基础
 */

/* 动态数组结构体 */
typedef struct DynamicArray {
    void** data;         // 存储数据指针的数组
    size_t size;         // 当前元素数量
    size_t capacity;     // 当前容量
} DynamicArray;

/* 
 * ========================================
 * 基础操作：创建和销毁
 * ========================================
 */

/**
 * 创建动态数组
 * @param initial_capacity 初始容量，0表示使用默认值
 * @return 新创建的动态数组指针，失败返回NULL
 */
DynamicArray* array_create(size_t initial_capacity);

/**
 * 销毁动态数组
 * @param array 要销毁的动态数组指针的地址
 * 注意：只释放数组本身，不释放存储的数据指针指向的内容
 */
void array_destroy(DynamicArray** array);

/* 
 * ========================================
 * 基本信息查询
 * ========================================
 */

/**
 * 获取数组大小
 * @param array 动态数组
 * @return 当前元素数量，NULL时返回0
 */
size_t array_size(const DynamicArray* array);

/**
 * 获取数组容量
 * @param array 动态数组
 * @return 当前容量，NULL时返回0
 */
size_t array_capacity(const DynamicArray* array);

/**
 * 检查数组是否为空
 * @param array 动态数组
 * @return true表示为空
 */
bool array_is_empty(const DynamicArray* array);

/* 
 * ========================================
 * 访问操作：获取和设置元素
 * ========================================
 */

/**
 * 通过索引访问元素
 * @param array 动态数组
 * @param index 索引位置
 * @return 元素指针，越界或失败返回NULL
 */
void* array_get_at(const DynamicArray* array, size_t index);

/**
 * 通过索引设置元素
 * @param array 动态数组
 * @param index 索引位置
 * @param data 要设置的数据指针
 * @return 成功返回true，失败返回false
 */
bool array_set_at(DynamicArray* array, size_t index, void* data);

/* 
 * ========================================
 * 核心操作：尾部插入和删除（重点学习）
 * ========================================
 */

/**
 * 在数组尾部添加元素（重点函数！）
 * @param array 动态数组
 * @param data 要添加的数据指针
 * @return 成功返回true，失败返回false
 * 
 * 这是学习的重点：理解扩容逻辑
 * 平均时间复杂度：O(1)，最坏情况：O(n)
 */
bool array_push_back(DynamicArray* array, void* data);

/**
 * 移除数组尾部元素
 * @param array 动态数组
 * @return 移除的元素指针，空数组返回NULL
 * 时间复杂度：O(1)
 */
void* array_pop_back(DynamicArray* array);

/* 
 * ========================================
 * 任意位置操作
 * ========================================
 */

/**
 * 在指定位置插入元素
 * @param array 动态数组
 * @param index 插入位置
 * @param data 要插入的数据指针
 * @return 成功返回true，失败返回false
 * 时间复杂度：O(n)
 */
bool array_insert_at(DynamicArray* array, size_t index, void* data);

/**
 * 移除指定位置的元素
 * @param array 动态数组
 * @param index 要移除的位置
 * @return 移除的元素指针，失败返回NULL
 * 时间复杂度：O(n)
 */
void* array_remove_at(DynamicArray* array, size_t index);

/* 
 * ========================================
 * 查找操作
 * ========================================
 */

/**
 * 查找元素第一次出现的位置
 * @param array 动态数组
 * @param data 要查找的数据指针
 * @param compare 比较函数
 * @return 找到返回索引，未找到返回-1
 * 时间复杂度：O(n)
 */
int array_find(const DynamicArray* array, const void* data, 
              int (*compare)(const void* a, const void* b));

/**
 * 检查数组是否包含指定元素
 * @param array 动态数组
 * @param data 要查找的数据指针
 * @param compare 比较函数
 * @return 包含返回true，不包含返回false
 */
bool array_contains(const DynamicArray* array, const void* data,
                   int (*compare)(const void* a, const void* b));

/* 
 * ========================================
 * 数组操作
 * ========================================
 */

/**
 * 清空数组（保持容量不变）
 * @param array 动态数组
 * 注意：只清空数组，不释放数据指针指向的内容
 */
void array_clear(DynamicArray* array);

/**
 * 反转数组元素顺序
 * @param array 动态数组
 */
void array_reverse(DynamicArray* array);

/**
 * 复制动态数组（浅拷贝）
 * @param array 源数组
 * @return 新的动态数组副本，失败返回NULL
 * 注意：只复制指针，不复制指针指向的数据
 */
DynamicArray* array_clone(const DynamicArray* array, void *(*clone_data)(const void *data));

/* 
 * ========================================
 * 调试和打印
 * ========================================
 */

/**
 * 打印数组内容
 * @param array 动态数组
 * @param print_func 元素打印函数
 */
void array_print(const DynamicArray* array, void (*print_func)(const void* data));

/**
 * 打印数组详细信息（size, capacity等）
 * @param array 动态数组
 */
void array_print_info(const DynamicArray* array);

#endif /* DYNAMIC_ARRAY_H */