#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef size_t (*hash_func_t)(const void *key, size_t keysz);
typedef int (*key_compare_t)(const void *k1, const void *k2);
typedef void (*key_destroy_t)(void *key);
typedef void (*value_destroy_t)(void *value);

typedef struct
{
    hash_func_t hash;
    key_compare_t eq;
    key_destroy_t destroy_key;
    value_destroy_t destroy_val;
} HashKeyOps;

typedef struct
{
    size_t total_elements;       /* 表中元素个数 */
    size_t used_buckets;         /* 被占用的桶数（开放寻址为非空槽位） */
    size_t max_chain_or_probe;   /* 链地址：最大链长；开放寻址：最大探测长度 */
    double average_chain_length; /* 链地址：平均链长；开放寻址：可置 0 或平均探测 */
    size_t collision_count;      /* 累计冲突次数 */
} HashStats;

typedef struct
{
    bool (*insert)(void *impl, const void *key, size_t keysz, void *val);
    void *(*search)(void *impl, const void *key, size_t keysz);
    bool (*erase)(void *impl, const void *key, size_t keysz);
    bool (*update)(void *impl, const void *key, size_t keysz, void *new_val);

    size_t (*size)(const void *impl);
    size_t (*capacity)(const void *impl);
    double (*load_factor)(const void *impl);
    HashStats (*stats)(const void *impl);

    void (*destroy)(void **impl);
} HashOps;

typedef struct HashTable
{
    void *impl;
    HashOps ops;
    HashKeyOps kops;
} HashTable;

HashTable *ht_create_from_impl(void *impl, HashOps ops, HashKeyOps kops);