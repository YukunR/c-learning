// hashtable.h
#pragma once
#include <stddef.h>
#include <stdbool.h>
#include "hashtable_internal.h"

/*
 * 统一的 HashTable 外部接口
 * - 创建请包含具体策略头文件：hashtable_chaining.h 或 hashtable_oa.h
 * - 这里提供增删改查、统计、便捷接口等
 */

/* ========= 通用 API ========= */
void hashtable_destroy(HashTable **ht);

/* CRUD */
bool hashtable_insert(HashTable *ht, const void *key, size_t keysz, void *value);
void *hashtable_search(HashTable *ht, const void *key, size_t keysz);
bool hashtable_delete(HashTable *ht, const void *key, size_t keysz);
bool hashtable_update(HashTable *ht, const void *key, size_t keysz, void *new_value);

/* 状态查询 */
size_t hashtable_size(const HashTable *ht);
size_t hashtable_capacity(const HashTable *ht);
double hashtable_load_factor(const HashTable *ht);
bool hashtable_is_empty(const HashTable *ht);

/* 统计与调试 */
HashStats hashtable_get_stats(const HashTable *ht);
/* 由实现打印更多内部统计信息（链长分布、墓碑数量等） */
void hashtable_print_stats(const HashTable *ht);

/* ========= 便捷接口（常用键类型） ========= */
/* 字符串（以 '\0' 结尾）；key_size 自动取 strlen(key)+1 */
bool hashtable_insert_string(HashTable *ht, const char *key, void *value);
void *hashtable_search_string(HashTable *ht, const char *key);
bool hashtable_delete_string(HashTable *ht, const char *key);

/* 整型键（按值拷贝） */
bool hashtable_insert_int(HashTable *ht, int key, void *value);
void *hashtable_search_int(HashTable *ht, int key);
bool hashtable_delete_int(HashTable *ht, int key);