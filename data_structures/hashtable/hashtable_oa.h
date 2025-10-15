// hashtable_oa.h
#pragma once
#include "hashtable.h"

/*
 * 开放地址法哈希表（线性/二次/双散列）
 * - 槽位数组（连续内存）+ SLOT_EMPTY/OCCUPIED/TOMBSTONE
 * - 通过探测函数选择策略；双散列需传入 secondary_hash（h2）
 */

typedef struct HashTableOA HashTableOA;

typedef enum
{
    PROBE_LINEAR = 0,
    PROBE_QUADRATIC = 1,
    PROBE_DOUBLEHASH = 2
} ProbeStrategy;

/**
 * 创建开放地址法哈希表
 * @param initial_capacity 初始容量（建议取素数或 2 的幂，视探测策略而定）
 * @param max_load_factor  负载因子阈值（如 0.5~0.8）；可另设墓碑比例阈值（在 .c 实现）
 * @param keyops           键相关回调（hash/eq/destroy）
 * @param probe            探测函数（线性/二次/双散列）
 * @param secondary_hash   双散列的 h2，可为 NULL（当非双散列时）
 *        典型用法：h2(key) % (capacity-1) + 1，且确保与 capacity 互质
 * @return 以统一接口 HashTable* 返回
 */
HashTable *hashtable_oa_create(
    size_t        initial_capacity,
    double        max_load_factor,
    HashKeyOps    keyops,
    ProbeStrategy probe,
    hash_func_t   secondary_hash /* 可为 NULL；双散列时必须提供 */
);
