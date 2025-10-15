#include "hash.h"

size_t hash_fnv1a(const void *key, size_t key_size) {
    const unsigned char *bytes = (const unsigned char *)key;
    size_t hash = 2166136261u;  // FNV offset basis

    for (size_t i = 0; i < key_size; i++) {
        hash ^= bytes[i];
        hash *= 16777619u;  // FNV prime
    }
    return hash;
}

size_t hash_division(const void *key, size_t key_size, size_t table_size) {
    return hash_fnv1a(key, key_size) % table_size;
}

size_t hash_multiplication(const void *key, size_t key_size, size_t table_size) {
    size_t key_hash = hash_fnv1a(key, key_size);
    // 使用整数运算模拟乘法散列（避免浮点运算）
    uint64_t hash_val = (uint64_t)key_hash * 2654435769u;  // 黄金比例 * 2^32
    return (size_t)((hash_val >> 32) * table_size >> 32);
}

size_t hash_string_djb2(const char *str) {
    size_t hash = 5381;  // 经过测试的质数
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

size_t hash_string_fnv1a(const char *str) {
    size_t hash = 2166136261u;    // FNV offset basis
    while (*str) {
        hash ^= (unsigned char)*str++;  // 先异或
        hash *= 16777619;               // 再乘 FNV prime
    }
    return hash;
}