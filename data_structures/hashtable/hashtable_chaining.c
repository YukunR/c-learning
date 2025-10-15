// data_structures/hashtable/hashtable_chaining.c

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "hashtable_chaining.h"
#include "hashtable_internal.h"
#include "../dynamic_array/dynamic_array.h"
#include "../doubly_circular_list/doubly_circular_list.h"

typedef struct HashTableChaining
{
    DynamicArray *buckets; // save
    size_t size;
    size_t capacity;
    double max_load_factor;
    size_t collision_count;
    HashKeyOps keyops;
} HashTableChaining;

typedef struct HashNode
{
    void *key;
    void *value;
    size_t key_size;
} HashNode;

static size_t next_capacity(size_t cur)
{
    return cur ? (cur << 1) : 8; // minimum capacity 8
}

static size_t prev_capacity(size_t cur)
{
    return (cur > 8) ? (cur >> 1) : 8;
}

static bool chaining_rehash(HashTableChaining *htc, size_t new_capacity, const HashKeyOps *keyops)
{
    if (new_capacity < 8)
        new_capacity = 8;

    // create new buckets
    DynamicArray *new_buckets = array_create(new_capacity);
    if (!new_buckets)
        return false;

    // init list
    for (size_t i = 0; i < new_capacity; i++)
    {
        DoublyCircularList *lst = list_create(keyops->eq);

        if (!lst)
        {
            for (size_t j = 0; j < i; j++)
            {
                DoublyCircularList *to_destroy = array_get_at(htc->buckets, j);
                if (to_destroy) {
                    list_destroy(&to_destroy);
                }
            }
            array_destroy(&new_buckets);
            return false;
        }

        array_push_back(new_buckets, lst);
    }

    // rehash
    htc->collision_count = 0;
    for (size_t i = 0; i < htc->capacity; i++)
    {
        DoublyCircularList *oldlst = (DoublyCircularList *)array_get_at(htc->buckets, i);

        size_t n = list_size(oldlst);
        for (size_t j = 0; j < n; j++)
        {
            HashNode *hn = (HashNode *)list_get_head(oldlst);
            list_remove_head(oldlst);

            size_t idx = keyops->hash(hn->key, hn->key_size) % new_capacity;
            DoublyCircularList *newlst = (DoublyCircularList *)array_get_at(new_buckets, idx);
            if (list_size(newlst) > 0)
                htc->collision_count++;
            list_insert_tail(newlst, hn);
        }
        list_destroy(&oldlst);
    }

    // replace bucket in hashtable
    array_destroy(&htc->buckets);
    htc->buckets = new_buckets;
    htc->capacity = new_capacity;

    return true;
}

static bool chaining_insert(void *impl, const void *key, size_t keysz, void *value)
{
    HashTableChaining *htc = (HashTableChaining *)impl;
    HashKeyOps *keyops = &htc->keyops;
    size_t idx = keyops->hash(key, keysz) % htc->capacity;
    DoublyCircularList *lst = (DoublyCircularList *)array_get_at(htc->buckets, idx);

    // find key and modify
    size_t n = list_size(lst);
    for (size_t i = 0; i < n; i++)
    {
        HashNode *hn = (HashNode *)list_get_at(lst, i);
        if (keyops->eq(key, hn->key) == 0)
        {
            hn->value = value;
            return true;
        }
    }
    if (n > 0)
    {
        htc->collision_count++; // not empty
    }

    // key doesn't exist
    HashNode *hn = malloc(sizeof(HashNode));
    if (!hn)
        return false;
    hn->key = malloc(keysz);
    if (!hn->key)
    {
        free(hn);
        return false;
    }
    memcpy(hn->key, key, keysz);
    hn->key_size = keysz;
    hn->value = value;

    // insert
    list_insert_tail(lst, hn);
    htc->size++;

    // rehash
    double alpha = (double)htc->size / (double)htc->capacity;
    if (alpha > htc->max_load_factor)
    {
        chaining_rehash(htc, next_capacity(htc->capacity), keyops);
    }
    else if (alpha < 0.25)
    {
        chaining_rehash(htc, prev_capacity(htc->capacity), keyops);
    }

    return true;
}

static inline size_t idx_for(HashTableChaining *htc, const void *key, size_t keysz)
{
    return htc->keyops.hash(key, keysz) % htc->capacity;
}

static void *chaining_search(void *impl, const void *key, size_t keysz)
{
    HashTableChaining *htc = (HashTableChaining *)impl;

    DoublyCircularList *lst = (DoublyCircularList *)array_get_at(htc->buckets, idx_for(htc, key, keysz));
    size_t n = list_size(lst);
    for (size_t i = 0; i < n; i++)
    {
        HashNode *hn = (HashNode *)list_get_at(lst, i);
        if (htc->keyops.eq(key, hn->key) == 0)
            return hn->value;
    }
    return NULL;
}

static bool chaining_erase(void *impl, const void *key, size_t keysz)
{
    HashTableChaining *htc = (HashTableChaining *)impl;
    HashKeyOps keyops = htc->keyops;

    DoublyCircularList *lst = (DoublyCircularList *)array_get_at(htc->buckets, idx_for(htc, key, keysz));
    size_t n = list_size(lst);
    for (size_t i = 0; i < n; i++)
    {
        HashNode *hn = (HashNode *)list_get_at(lst, i);
        if (keyops.eq(key, hn->key) == 0)
        {
            if (keyops.destroy_key)
                keyops.destroy_key(hn->key);
            else
                free(hn->key);
            if (keyops.destroy_val)
                keyops.destroy_val(hn->value);

            list_remove_at(lst, i);
            free(hn);
            htc->size--;
            return true;
        }
    }
    return false;
}

static bool chaining_update(void *impl, const void *key, size_t keysz, void *new_value)
{
    HashTableChaining *htc = (HashTableChaining *)impl;
    HashKeyOps keyops = htc->keyops;

    DoublyCircularList *lst = (DoublyCircularList *)array_get_at(htc->buckets, idx_for(htc, key, keysz));
    size_t n = list_size(lst);
    for (size_t i = 0; i < n; i++)
    {
        HashNode *hn = (HashNode *)list_get_at(lst, i);
        if (keyops.eq(key, hn->key) == 0)
        {
            hn->value = new_value;
            return true;
        }
    }
    return false;
}

static size_t chaining_size(const void *impl)
{
    return ((const HashTableChaining*)impl)->size;
}

static size_t chaining_capacity(const void *impl)
{
    return ((const HashTableChaining*)impl)->capacity;
}

static double chaining_load_factor(const void *impl)
{
    const HashTableChaining *htc = (const HashTableChaining*)impl;
    return (double)htc->size / htc->capacity;
}

static HashStats chaining_stats(const void *impl)
{
    HashTableChaining *htc = (const HashTableChaining*)impl;
    HashStats hs = {0};

    hs.total_elements = htc->size;
    hs.collision_count = htc->collision_count;
    hs.used_buckets = array_size(htc->buckets);

    // calculate average and max chain length
    size_t sum_length = 0;
    size_t max_length = 0;
    size_t n = array_capacity(htc->buckets);
    for (size_t i = 0; i < n; i++)
    {
        size_t chain_length = list_size(array_get_at(htc->buckets, i));
        sum_length += chain_length;
        if (max_length < chain_length)
        {
            max_length = chain_length;
        }
    }
    hs.average_chain_length = n ? (double)sum_length / n : 0.0;
    hs.max_chain_or_probe = max_length;

    return hs;
}

void chaining_destroy(void **pimpl)
{
    if (!pimpl || !*pimpl) return;
    HashTableChaining *htc = (HashTableChaining*)(*pimpl);
    if (!htc->buckets) {
        free(htc);
        *pimpl = NULL;
        return;
    }
    HashKeyOps keyops = htc->keyops;
    
    for (size_t i = 0; i < htc->buckets->size; i++)
    {
        DoublyCircularList *lst = array_get_at(htc->buckets, i);
        if (!lst) {
            continue;
        }
        while (!list_is_empty(lst)) 
        {
            HashNode *hn = (HashNode*)list_pop_front(lst);
            if (!hn) break;
            if (keyops.destroy_key) keyops.destroy_key(hn->key); 
            else if (hn->key) free(hn->key);
            if (keyops.destroy_val) keyops.destroy_val(hn->value);
            free(hn);
        }
        list_destroy((DoublyCircularList**)&(htc->buckets->data[i]));
    }
    array_destroy(&(htc->buckets));
    free(htc);
    *pimpl = NULL;
}

HashTable *hashtable_chaining_create(
    size_t initial_capacity,
    double max_load_factor,
    HashKeyOps keyops)
{
    if (initial_capacity == 0)
        initial_capacity = 8;
    HashTableChaining *impl = malloc(sizeof(HashTableChaining));
    if (!impl)
        return NULL;

    impl->max_load_factor = max_load_factor;
    impl->capacity = initial_capacity;
    impl->size = 0;
    impl->keyops = keyops;

    impl->buckets = array_create(initial_capacity);
    for (size_t i = 0; i < initial_capacity; i++)
    {
        DoublyCircularList *lst = list_create(keyops.eq);
        if (!lst)
        {
            for (size_t j = 0; j < i; j++)
            {
                DoublyCircularList *to_destroy = array_get_at(impl->buckets, j);
                if (to_destroy) {
                    list_destroy(&to_destroy);
                }
            }
            array_destroy(&impl->buckets);
            free(impl);
            return NULL;
        }
        array_push_back(impl->buckets, lst);
    }

    HashOps ops = {
        .insert = chaining_insert,
        .search = chaining_search,
        .erase = chaining_erase,
        .update = chaining_update,
        .size = chaining_size,
        .capacity = chaining_capacity,
        .load_factor = chaining_load_factor,
        .stats = chaining_stats,
        .destroy = chaining_destroy,
    };

    return ht_create_from_impl(impl, ops, keyops);
}