#include <stdlib.h>
#include "hashtable_internal.h"

HashTable *ht_create_from_impl(void *impl, HashOps ops, HashKeyOps kops)
{
    if (!impl)
        return NULL;
    HashTable *ht = malloc(sizeof(HashTable));
    if (!ht)
        return NULL;
    ht->impl = impl;
    ht->ops = ops;
    ht->kops = kops;
    return ht;
}