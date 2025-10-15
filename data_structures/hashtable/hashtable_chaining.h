// hashtable_chaining.h
#pragma once
#include "hashtable.h"

/*
 * 链地址法（拉链法）哈希表
 * 建议内部实现：
 * - buckets: DynamicArray*，每个元素为 DoublyCircularList*（链表头）
 * - 链表节点保存 {key, value, key_size}
 * - 负载因子触发扩容 + 重哈希
 */

typedef struct HashTableChaining HashTableChaining;

/**
 * 创建链地址法哈希表
 * @param initial_capacity 初始桶数（建议 >= 8）
 * @param max_load_factor  最大负载因子（常用 0.75）
 * @param keyops           键相关回调（hash/eq/destroy）
 * @return 以统一接口 HashTable* 返回
 */
HashTable *hashtable_chaining_create(
    size_t initial_capacity,
    double max_load_factor,
    HashKeyOps keyops
);