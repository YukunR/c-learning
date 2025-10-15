#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "hashtable_oa.h"
#include "hashtable_internal.h"

typedef enum
{
    SLOT_EMPTY = 0,
    SLOT_OCCUPIED = 1,
    SLOT_TOMBSTONE = 2
} SlotState;

typedef struct HashNode
{
    void *key;
    void *value;
    size_t keysz;
    SlotState state;
} HashNode;

typedef struct HashTableOA
{
    HashNode *table;
    size_t size;
    size_t capacity;
    size_t tombstones;
    double max_load_factor;
    ProbeStrategy probe;
    hash_func_t h1;
    hash_func_t h2;
    HashKeyOps keyops;
} HashTableOA;

static inline size_t gcd_size(size_t a, size_t b) {
    while (b) { size_t t = a % b; a = b; b = t; }
    return a;
}

static size_t next_capacity(size_t cur)
{
    return cur ? (cur << 1) : 8; // minimum capacity 8
}

static size_t prev_capacity(size_t cur)
{
    return (cur > 8) ? (cur >> 1) : 8;
}

static size_t next_idx(HashTableOA *htoa, size_t start, size_t step, size_t h2)
{
    size_t cap = htoa->capacity;
    if (htoa->probe == PROBE_LINEAR)
    {
        return (start % cap + step % cap) % cap;
    }
    else if (htoa->probe == PROBE_QUADRATIC)
    {
        size_t s = step % cap;
        return (start + (s * s) % cap) % cap;
    }
    else
    {
        // PROBE_DOUBLEHASH
        if (!h2)
            return SIZE_MAX;
        if (cap < 2) return SIZE_MAX;
        start %= cap;

        // 先得到一个 1..cap-1 的候选步长
        size_t step2 = 1 + (h2 % (cap - 1));

        // 关键：确保与 cap 互素
        if (gcd_size(step2, cap) != 1) {
            // 简单做法：逐步 +1 直到互素
            do {
                step2++;
                if (step2 == cap) step2 = 1; // 保持在 1..cap-1
            } while (gcd_size(step2, cap) != 1);
        }

        // 计算 (start + i * step2) % cap
        step %= cap; // 防溢也防无谓的大数运算
        size_t inc = ((step % cap) * (step2 % cap)) % cap;
        return (start + inc) % cap;
    }
}

static size_t idx_for(HashTableOA *htoa, const void *key, size_t keysz)
{
    if (!htoa)
        return SIZE_MAX;
    HashNode *table = htoa->table;
    size_t start = htoa->h1(key, keysz) % htoa->capacity;
    size_t first_tomb = SIZE_MAX;
    size_t h2 = htoa->h2 ? htoa->h2(key, keysz) : 0;
    for (size_t step = 0; step < htoa->capacity; step++)
    {
        size_t idx = next_idx(htoa, start, step, h2);
        if (table[idx].state == SLOT_OCCUPIED)
        {
            if (htoa->keyops.eq(table[idx].key, key) == 0)
            {
                return idx;
            }
        }
        else if (table[idx].state == SLOT_TOMBSTONE)
        {
            if (first_tomb == SIZE_MAX)
                first_tomb = idx;
        }
        else
        {
            // SLOT_EMPTY
            return (first_tomb == SIZE_MAX) ? idx : first_tomb;
        }
    }
    // the last tomb
    if (first_tomb != SIZE_MAX)
        return first_tomb;
    return SIZE_MAX;
}

static bool oa_rehash(HashTableOA *htoa, size_t new_capacity)
{
    if (!htoa)
        return false;
    HashNode *old_tab = htoa->table;
    size_t old_capacity = htoa->capacity;
    size_t old_size = htoa->size;
    size_t old_tombstones = htoa->tombstones;

    HashNode *new_tab = (HashNode *)calloc(new_capacity, sizeof(HashNode));
    if (!new_tab)
        return false;
    htoa->table = new_tab;
    htoa->capacity = new_capacity;
    htoa->size = 0;
    htoa->tombstones = 0;

    for (size_t i = 0; i < old_capacity; i++)
    {
        HashNode old_hn = old_tab[i];
        if (old_hn.state == SLOT_OCCUPIED)
        {
            size_t idx = idx_for(htoa, old_hn.key, old_hn.keysz);
            if (idx == SIZE_MAX)
            {
                free(new_tab);
                htoa->table = old_tab;
                htoa->capacity = old_capacity;
                htoa->size = old_size;
                htoa->tombstones = old_tombstones;
                return false;
            }
            HashNode *dst = &htoa->table[idx];
            dst->key = old_hn.key;
            dst->keysz = old_hn.keysz;
            dst->value = old_hn.value;
            dst->state = SLOT_OCCUPIED;
            htoa->size++;
        }
    }
    free(old_tab);
    return true;
}

static bool oa_insert(void *impl, const void *key, size_t keysz, void *val)
{
    if (!impl)
        return false;
    HashTableOA *htoa = (HashTableOA *)impl;
    HashKeyOps keyops = htoa->keyops;
    if ((double)(htoa->size + htoa->tombstones) / htoa->capacity > htoa->max_load_factor)
    {
        if (!oa_rehash(htoa, next_capacity(htoa->capacity)))
            return false;
    }

    size_t idx = idx_for(htoa, key, keysz);
    if (idx == SIZE_MAX)
        return false;
    HashNode *hn = &htoa->table[idx];
    if (hn->state == SLOT_OCCUPIED)
    {
        if (keyops.destroy_val && hn->value && hn->value != val)
        {
            keyops.destroy_val(hn->value);
        }
        hn->value = val;
        return true;
    }
    else
    {
        if (hn->state == SLOT_TOMBSTONE && htoa->tombstones)
            htoa->tombstones--;
        hn->key = malloc(keysz);
        if (!hn->key) return false;
        memcpy(hn->key, key, keysz);
        hn->keysz = keysz;
        hn->value = val;
        hn->state = SLOT_OCCUPIED;

        htoa->size++;
        return true;
    }
}

static void *oa_search(void *impl, const void *key, size_t keysz)
{
    if (!impl) return NULL;
    HashTableOA *htoa = (HashTableOA*)impl;
    HashNode *table = htoa->table;

    size_t idx = idx_for(htoa, key, keysz);
    if (idx == SIZE_MAX) return NULL;
    HashNode hn = table[idx];
    if (hn.state != SLOT_OCCUPIED) return NULL;
    if (htoa->keyops.eq(hn.key, key) != 0) return NULL;
    return hn.value;
}

static bool oa_erase(void *impl, const void *key, size_t keysz)
{
    if (!impl) return false;
    HashTableOA *htoa = (HashTableOA*)impl;
    HashNode *table = htoa->table;

    size_t idx = idx_for(htoa, key, keysz);
    if (idx == SIZE_MAX) return false;
    HashNode *hn = &table[idx];
    if (hn->state != SLOT_OCCUPIED) return false; 
    if (htoa->keyops.eq(hn->key, key) != 0) return false;

    if (htoa->keyops.destroy_key)
    {
        htoa->keyops.destroy_key(hn->key);
    } else {
        free(hn->key);
    }
    if (htoa->keyops.destroy_val)
    {
        htoa->keyops.destroy_val(hn->value);
    }
    
    hn->key = NULL;
    hn->value = NULL;
    hn->keysz = 0;
    hn->state = SLOT_TOMBSTONE;

    htoa->size--;
    htoa->tombstones++;
    return true;
}

static bool oa_update(void *impl, const void *key, size_t keysz, void *new_value)
{
    if (!impl) return false;
    HashTableOA *htoa = (HashTableOA*)impl;
    HashNode *table = htoa->table;
    size_t idx = idx_for(htoa, key, keysz);
    if (idx == SIZE_MAX) return false;
    HashNode *hn = &table[idx];
    if (hn->state != SLOT_OCCUPIED) return false;
    if (htoa->keyops.eq(key, hn->key) != 0) return false;

    if (hn->value == new_value) return true;
    if (htoa->keyops.destroy_val && hn->value) {
        htoa->keyops.destroy_val(hn->value);
    }
    hn->value = new_value;
    return true;
}

static size_t oa_size(const void *impl)
{
    if (!impl) return 0;
    return ((const HashTableOA*)impl)->size;
}

static size_t oa_capacity(const void *impl)
{
    if (!impl) return 0;
    return ((const HashTableOA*)impl)->capacity;
}

static double oa_load_factor(const void *impl)
{
    if (!impl) return -1.;
    const HashTableOA *htoa = (const HashTableOA *)impl;
    return ((double)htoa->size + htoa->tombstones) / htoa->capacity;
}

static HashStats oa_stats(const void *impl)
{
    printf("not implemented for OA!");
    HashStats hs={0};
    return hs;
}

void oa_destroy(void **pimpl)
{
    if (!pimpl || !*pimpl) return;
    HashTableOA *impl = *pimpl;
    HashNode *table = impl->table;
    size_t cap = impl->capacity;
    HashKeyOps keyops = impl->keyops;
    for (size_t i = 0; i < cap; i++)
    {
        HashNode *hn = &table[i];

        if (hn->state == SLOT_OCCUPIED) 
        {
            if (keyops.destroy_key) 
            {
                keyops.destroy_key(hn->key);
            }
            else
            {
                free(hn->key);
            }
            hn->key = NULL;
            
            if (keyops.destroy_val)
            {
                keyops.destroy_val(hn->value);
                hn->value = NULL;
            }

            hn = NULL;
        }
    }
    
    free(table);
    free(impl);
    *pimpl = NULL;
}

HashTable *hashtable_oa_create(
    size_t initial_capacity,
    double max_load_factor,
    HashKeyOps keyops,
    ProbeStrategy probe,
    hash_func_t secondary_hash /* 可为 NULL；双散列时必须提供 */
)
{
    if (initial_capacity == 0)
        initial_capacity = 8;
    HashTableOA *impl = malloc(sizeof(HashTableOA));
    if (!impl)
        return NULL;
    impl->table = (HashNode *)calloc(initial_capacity, sizeof(HashNode));
    if (!impl->table)
    {
        free(impl);
        return NULL;
    }
    impl->size = 0;
    impl->capacity = initial_capacity;
    impl->tombstones = 0;
    impl->max_load_factor = max_load_factor <= 0 ? 0.5 : max_load_factor;
    if (probe == PROBE_DOUBLEHASH && !secondary_hash)
    {
        free(impl->table);
        free(impl);
        return NULL;
    }
    impl->probe = probe ? probe : PROBE_LINEAR;
    impl->h1 = keyops.hash;
    impl->h2 = secondary_hash;
    impl->keyops = keyops;
    HashOps ops = {
        .insert = oa_insert,
        .search = oa_search,
        .erase = oa_erase,
        .update = oa_update,
        .size = oa_size,
        .capacity = oa_capacity,
        .load_factor = oa_load_factor,
        .stats = oa_stats,
        .destroy = oa_destroy,
    };

    return ht_create_from_impl(impl, ops, keyops);
}