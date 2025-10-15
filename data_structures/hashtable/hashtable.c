// data_structures/hashtable/hashtable.c

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "hashtable.h"
#include "hashtable_internal.h"

void hashtable_destroy(HashTable **ht)
{
    if (!ht ||!*ht) return;
    (*ht)->ops.destroy(&(*ht)->impl);
    free(*ht);
    *ht = NULL;
}

bool hashtable_insert(HashTable *ht, const void *key, size_t keysz, void *value) 
{
    if (!ht) return false;
    return ht->ops.insert(ht->impl, key, keysz, value);
}

void *hashtable_search(HashTable *ht, const void *key, size_t keysz)
{
    if (!ht) return NULL;
    return ht->ops.search(ht->impl, key, keysz);
}

bool hashtable_delete(HashTable *ht, const void *key, size_t keysz)
{
    if (!ht) return false;
    return ht->ops.erase(ht->impl, key, keysz);
}

bool hashtable_update(HashTable *ht, const void *key, size_t keysz, void *new_value)
{
    if (!ht) return false;
    return ht->ops.update(ht->impl, key, keysz, new_value);
}

size_t hashtable_size(const HashTable *ht) 
{
    if (!ht) return 0;
    return ht->ops.size(ht->impl);
}

size_t hashtable_capacity(const HashTable *ht) 
{
    if (!ht) return 0;
    return ht->ops.capacity(ht->impl);
}

double hashtable_load_factor(const HashTable *ht) 
{
    if (!ht) return -1.0;
    return ht->ops.load_factor(ht->impl);
}

bool hashtable_is_empty(const HashTable *ht) 
{
    if (!ht) return true;
    return ht->ops.size == 0;
}

HashStats hashtable_get_stats(const HashTable *ht)
{
    return(ht->ops.stats(ht->impl));
}

void hashtable_print_stats(const HashTable *ht)
{
    HashStats hashstats = ht->ops.stats(ht->impl);
    printf(
        "Hash Table Stats:\n"
        "    - Total elements: %zu\n"
        "    - Used buckets: %zu\n"
        "    - Max chain or probe: %zu\n"
        "    - Average chain length: %.2f\n"
        "    - Collision count: %zu", 
        hashstats.total_elements, 
        hashstats.used_buckets, 
        hashstats.max_chain_or_probe, 
        hashstats.average_chain_length, 
        hashstats.collision_count
    );
}

bool hashtable_insert_string(HashTable *ht, const char *key, void *value)
{
    return hashtable_insert(ht, key, key ? strlen(key) + 1 : 0, value);
}

void *hashtable_search_string(HashTable *ht, const char *key)
{
    return hashtable_search(ht, key, key ? strlen(key) + 1 : 0);
}

bool hashtable_delete_string(HashTable *ht, const char *key)
{
    return hashtable_delete(ht, key, key ? strlen(key) + 1 : 0);
}

bool hashtable_insert_int(HashTable *ht, int key, void *value)
{
    return hashtable_insert(ht, &key, sizeof(key), value);
}

void *hashtable_search_int(HashTable *ht, int key)
{
    return hashtable_search(ht, &key, sizeof(key));
}

bool hashtable_delete_int(HashTable *ht, int key)
{
    return hashtable_delete(ht, &key, sizeof(key));
}