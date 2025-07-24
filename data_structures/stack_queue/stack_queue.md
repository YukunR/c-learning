# 栈和队列实现指南

## 概述

栈（Stack）和队列（Queue）是两种基础的线性数据结构，它们的区别在于数据的存取顺序：

- **栈**：后进先出（LIFO - Last In First Out）
- **队列**：先进先出（FIFO - First In First Out）

基于你已实现的双向循环链表，我们可以高效地实现这两种数据结构。

## 栈（Stack）

### 基本概念

栈就像一摞盘子，只能从顶部添加或取走盘子：

```
    ↓ push    ↑ pop
    [  3  ] ← 栈顶 (top)
    [  2  ]
    [  1  ] ← 栈底 (bottom)
```

### 核心操作

| 操作       | 描述                       | 时间复杂度 |
| ---------- | -------------------------- | ---------- |
| `push`     | 压栈（添加元素到栈顶）     | O(1)       |
| `pop`      | 弹栈（移除并返回栈顶元素） | O(1)       |
| `peek/top` | 查看栈顶元素（不移除）     | O(1)       |
| `is_empty` | 检查栈是否为空             | O(1)       |
| `size`     | 获取栈中元素个数           | O(1)       |

### 基于双向循环链表的实现

利用你的双向循环链表，我们可以选择以下策略：

**策略 1：使用链表头作为栈顶**

- `push` → `list_insert_head`
- `pop` → `list_remove_head` + `list_get_head`
- `peek` → `list_get_head`

**策略 2：使用链表尾作为栈顶**

- `push` → `list_insert_tail`
- `pop` → `list_remove_tail` + `list_get_tail`
- `peek` → `list_get_tail`

**推荐使用策略 1**，因为通常头部操作在实现上更直观。

### 栈的应用场景

1. **函数调用栈**：程序执行时的函数调用管理
2. **表达式求值**：中缀转后缀、计算器实现
3. **撤销功能**：文本编辑器的 Ctrl+Z
4. **括号匹配**：检查代码中括号是否配对
5. **深度优先搜索（DFS）**：图和树的遍历
6. **浏览器历史**：后退功能的实现

### 接口设计

```c
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
```

## 队列（Queue）

### 基本概念

队列就像排队买票，先来的人先服务：

```
rear ← [3] [2] [1] ← front
        ↑           ↑
      enqueue    dequeue
     (入队)      (出队)
```

### 核心操作

| 操作       | 描述                       | 时间复杂度 |
| ---------- | -------------------------- | ---------- |
| `enqueue`  | 入队（在队尾添加元素）     | O(1)       |
| `dequeue`  | 出队（移除并返回队头元素） | O(1)       |
| `front`    | 查看队头元素（不移除）     | O(1)       |
| `rear`     | 查看队尾元素（不移除）     | O(1)       |
| `is_empty` | 检查队列是否为空           | O(1)       |
| `size`     | 获取队列中元素个数         | O(1)       |

### 基于双向循环链表的实现

双向循环链表非常适合实现队列：

- `enqueue` → `list_insert_tail`（在尾部入队）
- `dequeue` → `list_remove_head` + `list_get_head`（从头部出队）
- `front` → `list_get_head`
- `rear` → `list_get_tail`

这样的实现充分利用了双向循环链表两端操作都是 O(1) 的优势。

### 队列的应用场景

1. **任务调度**：操作系统的进程调度
2. **缓冲区**：IO 操作的数据缓冲
3. **广度优先搜索（BFS）**：图和树的层次遍历
4. **打印队列**：打印机任务管理
5. **消息队列**：系统间的异步通信
6. **流量控制**：网络数据包处理

### 接口设计

```c
// queue.h
#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct Queue Queue;

// 创建和销毁
Queue *queue_create(void);
void queue_destroy(Queue **queue);

// 队列操作
bool queue_enqueue(Queue *queue, void *data);
void *queue_dequeue(Queue *queue);
void *queue_front(Queue *queue);
void *queue_rear(Queue *queue);

// 状态查询
bool queue_is_empty(Queue *queue);
size_t queue_size(Queue *queue);

// 调试和工具
void queue_print(Queue *queue, void (*print_func)(const void *data));
void queue_clear(Queue *queue);

#endif
```

## 实现方案比较

### 基于双向循环链表 vs 基于动态数组

| 特性           | 双向循环链表          | 动态数组                   |
| -------------- | --------------------- | -------------------------- |
| **内存使用**   | 每个节点额外 2 个指针 | 连续内存，可能有未使用空间 |
| **缓存友好性** | 较差（指针跳跃）      | 较好（连续访问）           |
| **动态扩容**   | 无需扩容              | 需要重新分配和复制         |
| **实现复杂度** | 中等（已有链表）      | 简单                       |
| **栈实现**     | 优秀                  | 优秀                       |
| **队列实现**   | 优秀                  | 需要循环队列技巧           |

### 推荐选择

1. **栈**：两种实现都很好，建议先用链表实现（复用已有代码），后续可以尝试数组实现对比性能
2. **队列**：强烈推荐链表实现，因为双向循环链表天然支持双端操作

## 扩展：双端队列（Deque）

基于双向循环链表，你还可以轻松实现双端队列，它支持在两端都进行插入和删除：

```c
// deque.h 接口示例
bool deque_push_front(Deque *deque, void *data);
bool deque_push_back(Deque *deque, void *data);
void *deque_pop_front(Deque *deque);
void *deque_pop_back(Deque *deque);
```

对应的链表操作：

- `push_front` → `list_insert_head`
- `push_back` → `list_insert_tail`
- `pop_front` → `list_remove_head`
- `pop_back` → `list_remove_tail`

## 实现建议

### 文件组织

```
stack_queue/
├── stack/
│   ├── stack.h
│   ├── stack_list.c      # 基于链表的实现
│   ├── stack_array.c     # 基于数组的实现（可选）
│   └── test_stack.c
├── queue/
│   ├── queue.h
│   ├── queue_list.c      # 基于链表的实现
│   ├── queue_array.c     # 基于数组的实现（可选）
│   └── test_queue.c
└── deque/
    ├── deque.h
    ├── deque_list.c
    └── test_deque.c
```

### 实现步骤

1. **栈的实现**

   - 先实现基于链表的版本
   - 编写测试用例验证正确性
   - 可选：实现基于数组的版本并对比

2. **队列的实现**

   - 实现基于链表的版本
   - 重点测试 FIFO 特性
   - 可选：实现循环队列（基于数组）

3. **双端队列的实现**
   - 基于链表实现
   - 测试双端操作的正确性

### 测试用例建议

```c
// 测试栈
void test_stack_basic_operations();
void test_stack_empty_operations();
void test_stack_large_data();
void test_stack_memory_management();

// 测试队列
void test_queue_fifo_property();
void test_queue_empty_operations();
void test_queue_large_data();
void test_queue_memory_management();
```

### 性能测试

完成实现后，可以编写性能测试来比较不同实现：

```c
// 测试不同实现的性能
void benchmark_stack_operations(int num_operations);
void benchmark_queue_operations(int num_operations);
```

这样的实现不仅能巩固你的 C 语言基础，还能深入理解这些基础数据结构的本质，为后续学习哈希表、树等更复杂的数据结构打下坚实基础。
