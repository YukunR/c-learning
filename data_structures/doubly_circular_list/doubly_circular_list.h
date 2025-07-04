/**
 * doubly_circular_list.h
 */

#ifndef DOUBLY_CIRCULAR_LIST_H
#define DOUBLY_CIRCULAR_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 节点结构
typedef struct Node Node;

// 链表结构
typedef struct DoublyCircularList DoublyCircularList;

// ========== 基础操作 ==========

// 创建和销毁
DoublyCircularList* list_create(int (*compare)(const void* a, const void* b));
void list_destroy(DoublyCircularList **list);

// 插入操作
bool list_insert_head(DoublyCircularList* list, void* data);
bool list_insert_tail(DoublyCircularList* list, void* data);
bool list_insert_at(DoublyCircularList* list, size_t index, void* data);

// 删除操作
bool list_remove_head(DoublyCircularList* list);
bool list_remove_tail(DoublyCircularList* list);
bool list_remove_at(DoublyCircularList* list, size_t index);

// 访问操作
Node* list_get_node_at(DoublyCircularList* list, size_t index);
void* list_get_head(DoublyCircularList* list);
void* list_get_tail(DoublyCircularList* list);
void* list_get_at(DoublyCircularList* list, size_t index);

// 查找操作
int list_find(DoublyCircularList* list, const void* data);
bool list_contains(DoublyCircularList* list, const void* data);

// 基本信息
size_t list_size(DoublyCircularList* list);
bool list_is_empty(DoublyCircularList* list);

// 遍历操作
void list_print_forward(DoublyCircularList* list, void (*print_func)(const void* data));
void list_print_backward(DoublyCircularList* list, void (*print_func)(const void* data));
#endif