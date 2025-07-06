/**
 * common.c - Implement of commen.h
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// compare functions
int compare_int(const void *a, const void *b)
{
    int val_a = *(const int *)a;
    int val_b = *(const int *)b;

    if (val_a < val_b)
        return -1;
    if (val_a > val_b)
        return 1;
    return 0; // if val_a == val_b
}

int compare_string(const void *a, const void *b)
{
    const char *str_a = (const char *)a;
    const char *str_b = (const char *)b;

    return strcmp(str_a, str_b);
}

int compare_float(const void *a, const void *b)
{
    float val_a = *(const float *)a;
    float val_b = *(const float *)b;

    if (val_a < val_b)
        return -1;
    if (val_a > val_b)
        return 1;
    return 0; // if val_a == val_b
}

int compare_double(const void *a, const void *b)
{
    double val_a = *(const double *)a;
    double val_b = *(const double *)b;

    if (val_a < val_b)
        return -1;
    if (val_a > val_b)
        return 1;
    return 0; // if val_a == val_b
}

// clone function
void *clone_int(const void *data)
{
    int *new_data = malloc(sizeof(int));
    if (new_data == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for int");
        return NULL;
    }
    *new_data = *(int *)data;
    return new_data;
}

void *clone_string(const void *data)
{
    const char *src = (const char *)data;
    size_t len = strlen(src);

    char *new_data = malloc(len + 1);
    if (new_data == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for int");
        return NULL;
    }

    strcpy(new_data, src);
    return new_data;
}

// print function
void print_int(const void *data)
{
    if (!data)
    {
        printf("(null) ");
        return;
    }

    printf("%d ", *(const int *)data);
}