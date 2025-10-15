#ifndef HASH_H
#define HASH_H

#include <stdint.h>

size_t hash_fnv1a(const void *key, size_t key_size); 
size_t hash_division(const void *key, size_t key_size, size_t table_size);
size_t hash_multiplication(const void *key, size_t key_size, size_t table_size);

size_t hash_string_djb2(const char *str);
size_t hash_string_fnv1a(const char *str);

#endif