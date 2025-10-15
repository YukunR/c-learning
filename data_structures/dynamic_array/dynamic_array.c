/*
Implement of dynamic array.

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dynamic_array.h"
#include "../common/common.h"

// init and destroy
DynamicArray *array_create(size_t initial_capacity)
{
    if (initial_capacity == 0)
    {
        // use 2 as default capacity
        initial_capacity = 2;
    }

    DynamicArray *new_array = malloc(sizeof(DynamicArray));
    if (new_array == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for DynamicArray\n");
        return NULL;
    }

    new_array->data = malloc(initial_capacity * sizeof(void *));
    if (new_array->data == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for data\n");
        free(new_array);
        return NULL;
    }
    new_array->capacity = initial_capacity;
    new_array->size = 0;

    return new_array;
}

void array_destroy(DynamicArray **array)
{
    if (!array || !*array)
    {
        fprintf(stderr, "Array doesn't exist\n");
        return;
    }

    free((*array)->data);
    free(*array);
    *array = NULL;
}

size_t array_size(const DynamicArray *array)
{
    if (!array)
    {
        fprintf(stderr, "Array doesn't exist\n");
        return -1;
    }

    return array->size;
}

size_t array_capacity(const DynamicArray *array)
{
    if (!array)
    {
        fprintf(stderr, "Array doesn't exist\n");
        return -1;
    }

    return array->capacity;
}

bool array_is_empty(const DynamicArray *array)
{
    if (!array)
    {
        fprintf(stderr, "Array doesn't exist\n");
        return false;
    }

    return array->size == 0;
}

// insert, delete
bool array_push_back(DynamicArray *array, void *data)
{
    if (!array)
    {
        fprintf(stderr, "Array doesn't exist\n");
        return false;
    }

    if (array->size == array->capacity)
    {
        if (array->capacity > SIZE_MAX / 2)
        {
            fprintf(stderr, "Exceeded maximum array capacity\n");
            return false;
        }
        size_t new_capacity = array->capacity * 2;
        void **new_data = realloc(array->data, new_capacity * sizeof(void *));
        if (!new_data)
        {
            fprintf(stderr, "Failed to reallocate memory for data\n");
            return false;
        }
        // copy to new mem
        array->data = new_data;
        array->capacity = new_capacity;
    }

    array->data[array->size] = data;
    array->size++;

    return true;
}

void *array_pop_back(DynamicArray *array)
{
    if (!array)
    {
        fprintf(stderr, "Array doesn't exist\n");
        return NULL;
    }
    if (array->size == 0)
    {
        fprintf(stderr, "Array is empty\n");
        return NULL;
    }

    // pop data
    size_t capacity = array->capacity;
    void *data = array->data[array->size - 1];
    array->size--;

    array->data[array->size] = NULL;

    // shrink
    if (array->size < capacity / 4 && array->capacity > 1)
    {
        size_t new_capacity = capacity / 2;
        void **new_data = realloc(array->data, new_capacity * sizeof(void *));
        if (!new_data)
        {
            fprintf(stderr, "Shrink failed: memory reallocation failed\n");
            return data;
        }
        array->data = new_data;
        array->capacity = new_capacity;
    }

    return data;
}

bool array_insert_at(DynamicArray *array, size_t index, void *data)
{
    if (!array)
    {
        fprintf(stderr, "Array doesn't exist\n");
        return false;
    }
    if (index > array->size)
    {
        fprintf(stderr, "Index %zu out of bounds [0, %zu]\n", index, array->size);
        return false;
    }

    if (array->size == array->capacity)
    {
        if (array->capacity > SIZE_MAX / 2)
        {
            fprintf(stderr, "Exceeded maximum array capacity\n");
            return false;
        }
        size_t new_capacity = array->capacity * 2;
        void **new_data = realloc(array->data, new_capacity * sizeof(void *));
        if (!new_data)
        {
            fprintf(stderr, "Failed to reallocate memory for data\n");
            return false;
        }
        // copy to new mem
        array->data = new_data;
        array->capacity = new_capacity;
    }

    if (index < array->size)
    {
        memmove(&array->data[index + 1], &array->data[index],
                (array->size - index) * sizeof(void *));
    }
    array->data[index] = data;
    array->size++;

    return true;
}

void *array_remove_at(DynamicArray *array, size_t index)
{
    if (!array)
    {
        fprintf(stderr, "Array doesn't exist\n");
        return NULL;
    }
    if (index >= array->size)
    {
        fprintf(stderr, "Index %zu out of bounds [0, %zu)\n", index, array->size);
        return NULL;
    }

    // remove data
    size_t capacity = array->capacity;
    memmove(
        &array->data[index], &array->data[index + 1],
        (array->size - index) * sizeof(void *));

    array->data[array->size - 1] = NULL;
    array->size--;

    // shrink
    if (array->size < capacity / 4 && array->capacity > 1)
    {
        size_t new_capacity = capacity / 2;
        void **new_data = realloc(array->data, new_capacity * sizeof(void *));
        if (new_data)
        {
            array->data = new_data;
            array->capacity = new_capacity;
        }
        else
        {
            fprintf(stderr, "Shrink failed: memory reallocation failed\n");
        }
    }
}

// get functions
void *array_get_at(const DynamicArray *array, size_t index)
{
    if (!array)
    {
        fprintf(stderr, "Array doesn't exist\n");
        return NULL;
    }
    if (index >= array->size)
    {
        fprintf(stderr, "Index %zu out of bounds [0, %zu)\n", index, array->size);
        return NULL;
    }

    void *element = array->data[index];
    if (!element)
    {
        fprintf(stderr, "Warning: element at index %zu is NULL\n", index);
    }
    return element;
}

// find
int array_find(const DynamicArray *array, const void *data,
               int (*compare)(const void *a, const void *b))
{
    if (!array)
    {
        fprintf(stderr, "Array doesn't exist\n");
        return -1;
    }
    if (array->size == 0)
    {
        fprintf(stderr, "Array is empty\n");
        return -1;
    }
    if (!compare)
    {
        fprintf(stderr, "No compare function\n");
        return -1;
    }

    for (int i = 0; i < array->size; i++)
    {
        if (compare(array->data[i], data) == 0)
            return i;
    }

    return -1;
}

bool array_contains(const DynamicArray *array, const void *data,
                    int (*compare)(const void *a, const void *b))
{
    if (array_find(array, data, compare) >= 0)
        return true;
    return false;
}

bool array_set_at(DynamicArray *array, size_t index, void *data)
{
    if (!array)
    {
        fprintf(stderr, "Array doesn't exist\n");
        return false;
    }
    if (index >= array->size)
    {
        fprintf(stderr, "Index %zu out of bounds [0, %zu)\n", index, array->size);
        return false;
    }
    array->data[index] = data;
    return true;
}

// print func
void array_print(const DynamicArray *array, void (*print_func)(const void *data))
{
    if (!array)
    {
        fprintf(stderr, "Array is NULL\n");
        return;
    }

    if (!print_func)
    {
        fprintf(stderr, "print_func is NULL\n");
        return;
    }

    printf("[");
    for (size_t i = 0; i < array->size; i++)
    {
        print_func(array->data[i]);
        if (i < array->size - 1)
            printf(", ");
    }
    printf("]\n");
}

// operate
void array_clear(DynamicArray *array)
{
    if (!array)
    {
        fprintf(stderr, "Array is NULL\n");
        return;
    }

    for (int i = 0; i < array->size; i++)
    {
        array->data[i] = NULL;
    }

    array->size = 0;
}

void array_reverse(DynamicArray *array)
{
    if (!array)
    {
        fprintf(stderr, "Array is NULL\n");
        return;
    }

    for (size_t i = 0, j = array->size - 1; i < j; i++, j--)
    {
        void *tmp = array->data[i];
        array->data[i] = array->data[j];
        array->data[j] = tmp;
    }
}

DynamicArray *array_clone(const DynamicArray *array, void *(*clone_data)(const void *data))
{
    if (!array)
    {
        fprintf(stderr, "Array is NULL\n");
        return NULL;
    }

    DynamicArray *new_array = array_create(array->capacity);
    for (int i = 0; i < array->size; i++)
    {
        new_array->data[i] = clone_data ? clone_data(array->data[i]) : array->data[i];
    }
    new_array->size = array->size;

    return new_array;
}