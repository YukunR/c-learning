// stack.c

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "../../doubly_circular_list/doubly_circular_list.h"

struct Stack
{
    DoublyCircularList *list;
};

Stack *stack_create(void)
{
    Stack *stack = malloc(sizeof(Stack));
    if (!stack)
    {
        fprintf(stderr, "Failed to allocate memory for Stack\n");
        return NULL;
    }
    stack->list = list_create(NULL);
    if (!stack->list)
    {
        fprintf(stderr, "Failed to create underlying list for Stack\n");
        free(stack);
        return NULL;
    }

    return stack;
}

void stack_destroy(Stack **stack)
{
    if (!stack || !*stack)
    {
        return;
    }

    list_destroy(&(*stack)->list);
    free(*stack);
    *stack = NULL;
}

bool stack_push(Stack *stack, void *data)
{
    if (!stack)
    {
        fprintf(stderr, "Stack doesn't exist\n");
        return false;
    }

    return list_insert_tail(stack->list, data);
}

void *stack_pop(Stack *stack)
{
    if (!stack)
    {
        fprintf(stderr, "Stack doesn't exist\n");
        return NULL;
    }

    void *data = list_get_tail(stack->list);
    list_remove_tail(stack->list);
    return data;
}

void *stack_peek(Stack *stack)
{
    if (!stack)
    {
        fprintf(stderr, "Stack doesn't exist\n");
        return NULL;
    }

    return list_get_tail(stack->list);
}

bool stack_is_empty(Stack *stack)
{
    if (!stack)
    {
        fprintf(stderr, "Stack doesn't exist\n");
        return true;
    }
    return list_is_empty(stack->list);
}

size_t stack_size(Stack *stack)
{
    if (!stack)
    {
        fprintf(stderr, "Stack doesn't exist\n");
        return 0;
    }
    return list_size(stack->list);
}

void stack_print(Stack *stack, void (*print_func)(const void *data))
{
    if (!stack)
    {
        fprintf(stderr, "Stack doesn't exist\n");
        return;
    }
    list_print_forward(stack->list, print_func);
}

void stack_clear(Stack *stack)
{
    if (!stack)
    {
        fprintf(stderr, "Stack doesn't exist\n");
        return;
    }
    list_clear(stack->list);
}
