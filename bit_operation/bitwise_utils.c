#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitwise_utils.h"

bool bit_is_set(uint32_t value, int n)
{
    if (n < 0 || n >= 32)
        return false;
    return (value & (1 << n)) != 0;
}

uint32_t bit_set(uint32_t value, int n)
{
    if (n < 0 || n >= 32)
        return value;
    return value | (1 << n);
}

uint32_t bit_clear(uint32_t value, int n)
{
    if (n < 0 || n >= 32)
        return value;
    return value & ~(1 << n);
}

uint32_t bit_flip(uint32_t value, int n)
{
    if (n < 0 || n >= 32)
        return value;
    return value ^ (1 << n);
}

uint32_t get_low_bits(uint32_t value, int n)
{
    if (n < 0 || n >= 32)
        return value;
    return value & ((1 << n) - 1);
}

int count_set_bits(uint32_t value)
{
    int count = 0;
    while (value != 0)
    {
        value &= (value - 1);
        count++;
    }
    return count;
}

int count_clear_bits(uint32_t value)
{
    int count = 0;
    while (value + 1 != 0)
    {
        value |= (value + 1);
        count++;
    }
    return count;
}

int find_rightmost_set_bit(uint32_t value)
{
    int pos = 0;
    while (value != 0)
    {
        if (value & 1)
        {
            return pos;
        }
        value >>= 1;
        pos++;
    }
    return -1;
}

int find_leftmost_set_bit(uint32_t value)
{
    uint32_t mask = 1 << 31;
    int pos = 31;
    while (value != 0)
    {
        if (value & mask)
        {
            return pos;
        }
        value <<= 1;
        pos--;
    }
    return -1;
}

bool is_power_of_2(uint32_t value)
{
    return value > 0 && (value & (value - 1)) == 0;
}

uint32_t reverse_bits(uint32_t value)
{
    uint32_t result = 0;
    for (int i = 0; i < 32; i++)
    {
        result = (result << 1) | (value & 1);
        value >>= 1;
    }
    return result;
}

uint32_t rotate_left(uint32_t value, int n)
{
    n %= 32;
    return (value << n) | (value >> (32 - n));
}

uint32_t rotate_right(uint32_t value, int n)
{
    n %= 32;
    return (value >> n) | (value << (32 - n));
}

uint32_t swap_bits(uint32_t value, int i, int j)
{
    if (i >= 32 || j >= 32)
    {
        return value;
    }

    if (((value >> i) & 1) != ((value >> j) & 1))
    {
        value ^= (1 << i) | (1 << j);
    }

    return value;
}

uint32_t extract_bits(uint32_t value, int position, int length)
{
    if (position >= 32 || position < 0)
    {
        return value;
    }

    length = (position + length < 32) ? length : 32 - position;
    return (value >> position) & ((1U << length) - 1);
}

void print_binary(unsigned int n, int bits)
{
    for (int i = bits - 1; i >= 0; i--)
    {
        printf("%d", (n >> i) & 1);
        if (i % 4 == 0 && i != 0)
            printf(" "); // 每4位添加空格
    }
    printf("\n");
}

int hamming_distance(uint32_t a, uint32_t b)
{
    uint32_t flag = a ^ b;
    int distance = 0;
    while (flag)
    {
        flag &= (flag - 1);
        distance++;
    }

    return distance;
}

bool is_even(uint32_t value)
{
    return (value & 1 == 0);
}

void swap_without_temp(uint32_t *a, uint32_t *b)
{
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

uint32_t multiply_by_power_of_2(uint32_t value, int n)
{
    return value << n;
}

// flag manager
FlagManager *flag_manager_create(void)
{
    FlagManager *fm = malloc(sizeof(FlagManager));
    if (fm)
    {
        fm->flags = FLAG_NONE;
    }

    return fm;
}

void flag_manager_destroy(FlagManager **manager)
{
    if (*manager)
    {
        free(*manager);
        *manager = NULL;
    }
}

void flag_set(FlagManager *manager, ObjectFlags flag)
{
    manager->flags |= flag;
}

void flag_clear(FlagManager *manager, ObjectFlags flag)
{
    manager->flags &= (~flag);
}

bool flag_is_set(FlagManager *manager, ObjectFlags flag)
{
    return (manager->flags & flag) != 0;
}

void flag_toggle(FlagManager *manager, ObjectFlags flag)
{
    manager->flags ^= flag;
}

void flag_set_multiple(FlagManager *manager, uint32_t flags)
{
    flags &= FLAG_ALL;
    manager->flags |= flags;
}

void flag_clear_all(FlagManager *manager)
{
    manager->flags = FLAG_NONE;
}

char *flag_to_string(FlagManager *manager)
{
    static char s_flag[6];
    for (int i = 4; i >= 0; i--)
    {
        s_flag[i] = ((manager->flags >> i) & 1) ? '1' : '0';
    }
    s_flag[5] = '\0';
    return s_flag;
}

bool flag_equals(FlagManager *a, FlagManager *b)
{
    return a->flags == b->flags;
}

BitArray *bitarray_create(size_t count, int bits_per_item)
{
    bits_per_item = bits_per_item <= 32 ? bits_per_item : 32;
    size_t total_bits = count * bits_per_item;
    size_t total_bytes = (total_bits + 7) / 8;
    uint8_t *data = malloc(total_bytes);
    if (!data)
        return NULL;

    BitArray *bitarray = malloc(sizeof(BitArray));
    if (!bitarray)
    {
        free(data);
        return NULL;
    }
    bitarray->bits_per_item = bits_per_item;
    bitarray->capacity = total_bits;
    bitarray->data = data;

    return bitarray;
}

void bitarray_destroy(BitArray **array)
{
    if (!array || !*array)
        return;

    free((*array)->data);
    free(*array);
    *array = NULL;
}

void bitarray_set(BitArray *array, size_t index, uint32_t value)
{
    if (!array)
        return;
    if ((value >> array->bits_per_item) != 0)
    {
        fprintf(stderr, "Warning: value %d exceeds %d bits and may cause precision loss.\n", value, array->bits_per_item);
        uint32_t mask = (array->bits_per_item == 32) ? 0xFFFFFFFF : ((1U << array->bits_per_item) - 1);
        value &= mask;
    }

    size_t start_bit_offset = index * array->bits_per_item;
    size_t start_byte_index = start_bit_offset / 8;
    size_t start_bit_in_byte = start_bit_offset % 8;

    if (start_bit_offset + array->bits_per_item > array->capacity)
        return;

    size_t bits_remaining = array->bits_per_item;

    for (size_t byte_idx = start_byte_index; bits_remaining > 0; byte_idx++)
    {
        size_t bit_pos_in_byte = (byte_idx == start_byte_index) ? start_bit_in_byte : 0;
        size_t bits_to_write = (bits_remaining + bit_pos_in_byte > 8) ? (8 - bit_pos_in_byte) : bits_remaining;

        // clear target pos
        uint8_t clear_mask = ((1U << bits_to_write) - 1) << bit_pos_in_byte;
        array->data[byte_idx] &= ~clear_mask;

        // set new value
        uint8_t value_part = (value >> (array->bits_per_item - bits_remaining)) & ((1U << bits_to_write) - 1);
        array->data[byte_idx] |= value_part << bit_pos_in_byte;

        bits_remaining -= bits_to_write;
    }
}

uint32_t bitarray_get(BitArray *array, size_t index)
{
    if (!array)
        return 0;

    size_t start_bit_offset = index * array->bits_per_item;
    if (start_bit_offset + array->bits_per_item > array->capacity)
        return 0;

    uint32_t value = 0;
    size_t start_byte_index = start_bit_offset / 8;
    size_t start_bit_in_byte = start_bit_offset % 8;
    size_t bits_remaining = array->bits_per_item;

    for (size_t byte_idx = start_byte_index; bits_remaining > 0; byte_idx++)
    {
        size_t bit_pos_in_byte = (byte_idx == start_byte_index) ? start_bit_in_byte : 0;
        size_t bits_to_get = (bits_remaining + bit_pos_in_byte > 8) ? (8 - bit_pos_in_byte) : bits_remaining;

        // get value
        uint8_t value_part = (array->data[byte_idx] >> bit_pos_in_byte) & ((1U << bits_to_get) - 1);
        value |= ((uint32_t)value_part << (array->bits_per_item - bits_remaining));

        bits_remaining -= bits_to_get;
    }
    return value;
}

void xor_encrypt_decrypt(uint8_t *data, size_t length, const char *key)
{
    if (!data || !key) return;

    size_t key_len = strlen(key);
    if (key_len == 0) return;

    for (int i = 0; i < length; i++)
    {
        data[i] ^= key[i % key_len];
    }
}