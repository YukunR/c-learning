// stack.h
#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stddef.h>

typedef struct Stack Stack;

// 创建和销毁
Stack *stack_create(void);
void stack_destroy(Stack **stack);

// 栈操作
bool stack_push(Stack *stack, void *data);
void *stack_pop(Stack *stack);
void *stack_peek(Stack *stack);

// 状态查询
bool stack_is_empty(Stack *stack);
size_t stack_size(Stack *stack);

// 调试和工具
void stack_print(Stack *stack, void (*print_func)(const void *data));
void stack_clear(Stack *stack);

#endif