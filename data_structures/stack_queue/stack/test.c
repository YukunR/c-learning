#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "stack.h"
#include "../../commen/commen.h"

void test_stack_basic_operations()
{
    printf("=== test_stack_basic_operations ===\n");

    Stack *stack = stack_create();
    assert(stack != NULL);
    assert(stack_is_empty(stack));

    int a = 10, b = 20, c = 30;
    assert(stack_push(stack, &a));
    assert(stack_push(stack, &b));
    assert(stack_push(stack, &c));
    assert(stack_size(stack) == 3);

    int *top = (int *)stack_peek(stack);
    assert(top && *top == 30);

    int *p1 = (int *)stack_pop(stack);
    assert(p1 && *p1 == 30);

    int *p2 = (int *)stack_pop(stack);
    assert(p2 && *p2 == 20);

    int *p3 = (int *)stack_pop(stack);
    assert(p3 && *p3 == 10);

    assert(stack_is_empty(stack));

    stack_destroy(&stack);
    assert(stack == NULL);

    printf("Passed ✅\n\n");
}

void test_stack_empty_operations()
{
    printf("=== test_stack_empty_operations ===\n");

    Stack *stack = stack_create();
    assert(stack != NULL);

    assert(stack_is_empty(stack));
    assert(stack_size(stack) == 0);

    assert(stack_peek(stack) == NULL);
    assert(stack_pop(stack) == NULL);

    stack_destroy(&stack);
    printf("Passed ✅\n\n");
}

void test_stack_large_data()
{
    printf("=== test_stack_large_data ===\n");

    Stack *stack = stack_create();
    assert(stack != NULL);

    const int N = 1000;
    int *nums = malloc(sizeof(int) * N);
    for (int i = 0; i < N; ++i)
    {
        nums[i] = i;
        assert(stack_push(stack, &nums[i]));
    }
    assert(stack_size(stack) == N);

    for (int i = N - 1; i >= 0; --i)
    {
        int *val = (int *)stack_pop(stack);
        assert(val && *val == i);
    }
    assert(stack_is_empty(stack));
    free(nums);

    stack_destroy(&stack);
    printf("Passed ✅\n\n");
}

void test_stack_memory_management()
{
    printf("=== test_stack_memory_management ===\n");

    Stack *stack = stack_create();
    assert(stack != NULL);

    int a = 1, b = 2;
    stack_push(stack, &a);
    stack_push(stack, &b);

    stack_clear(stack);
    assert(stack_is_empty(stack));

    stack_destroy(&stack);
    assert(stack == NULL);

    printf("Passed ✅\n\n");
}

int main()
{
    test_stack_basic_operations();
    test_stack_empty_operations();
    test_stack_large_data();
    test_stack_memory_management();
    return 0;
}