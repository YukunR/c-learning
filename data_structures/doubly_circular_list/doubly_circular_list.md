# 双向循环链表实现指南

## 1. 数据结构特征

### 基本概念

双向循环链表（Doubly Circular Linked List）是一种特殊的链表结构，具有以下特征：

- **双向性**：每个节点都有指向前一个节点（prev）和后一个节点（next）的指针
- **循环性**：最后一个节点的 next 指向第一个节点，第一个节点的 prev 指向最后一个节点
- **无明确头尾**：由于循环特性，可以从任意节点开始遍历整个链表

### 结构图示

```
    ┌─────┐    ┌─────┐    ┌─────┐    ┌─────┐
    │  A  │◄──►│  B  │◄──►│  C  │◄──►│  D  │
    └─────┘    └─────┘    └─────┘    └─────┘
       ▲                                 │
       └─────────────────────────────────┘
```

### 与 Python list 的对比

- Python 的 list 底层是动态数组，支持随机访问但插入删除开销大
- 双向循环链表插入删除 O(1)，但不支持随机访问（需要 O(n)遍历）

## 2. 数据结构定义

### 节点结构

```c
typedef struct DoublyNode {
    void* data;                    // 数据域（使用void*支持任意数据类型）
    struct DoublyNode* prev;       // 指向前一个节点
    struct DoublyNode* next;       // 指向后一个节点
} DoublyNode;
```

### 链表结构

```c
typedef struct DoublyCircularList {
    DoublyNode* head;              // 头节点指针（可以是任意节点）
    size_t size;                   // 链表中节点的数量

    // 可选：比较函数，用于查找和排序
    int (*compare)(const void* a, const void* b);

    // 可选：释放函数，用于释放data指向的内存
    void (*free_data)(void* data);
} DoublyCircularList;
```

## 3. 核心功能实现

### 3.1 基础操作

#### 创建和销毁

```c
// 创建空链表
DoublyCircularList* dclist_create(void);

// 创建带比较函数的链表
DoublyCircularList* dclist_create_with_compare(
    int (*compare)(const void* a, const void* b)
);

// 销毁整个链表
void dclist_destroy(DoublyCircularList* list);

// 创建新节点
DoublyNode* dclist_node_create(void* data);

// 销毁单个节点
void dclist_node_destroy(DoublyNode* node, void (*free_data)(void*));
```

#### 插入操作

```c
// 在链表头部插入
bool dclist_insert_head(DoublyCircularList* list, void* data);

// 在链表尾部插入
bool dclist_insert_tail(DoublyCircularList* list, void* data);

// 在指定位置插入（index从0开始）
bool dclist_insert_at(DoublyCircularList* list, size_t index, void* data);

// 在指定节点之前插入
bool dclist_insert_before(DoublyCircularList* list, DoublyNode* target, void* data);

// 在指定节点之后插入
bool dclist_insert_after(DoublyCircularList* list, DoublyNode* target, void* data);
```

#### 删除操作

```c
// 删除头节点
bool dclist_remove_head(DoublyCircularList* list);

// 删除尾节点
bool dclist_remove_tail(DoublyCircularList* list);

// 删除指定位置的节点
bool dclist_remove_at(DoublyCircularList* list, size_t index);

// 删除指定节点
bool dclist_remove_node(DoublyCircularList* list, DoublyNode* target);

// 删除第一个匹配的数据
bool dclist_remove_data(DoublyCircularList* list, const void* data);

// 清空链表（保留链表结构）
void dclist_clear(DoublyCircularList* list);
```

### 3.2 查找和访问

```c
// 获取指定位置的数据
void* dclist_get_at(const DoublyCircularList* list, size_t index);

// 获取头节点数据
void* dclist_get_head(const DoublyCircularList* list);

// 获取尾节点数据
void* dclist_get_tail(const DoublyCircularList* list);

// 查找数据，返回第一个匹配的节点
DoublyNode* dclist_find(const DoublyCircularList* list, const void* data);

// 查找数据的索引位置
int dclist_index_of(const DoublyCircularList* list, const void* data);

// 检查是否包含指定数据
bool dclist_contains(const DoublyCircularList* list, const void* data);
```

### 3.3 状态查询

```c
// 获取链表大小
size_t dclist_size(const DoublyCircularList* list);

// 检查链表是否为空
bool dclist_is_empty(const DoublyCircularList* list);

// 验证链表结构完整性（调试用）
bool dclist_is_valid(const DoublyCircularList* list);
```

### 3.4 遍历操作

```c
// 正向遍历（从head开始）
void dclist_foreach_forward(const DoublyCircularList* list,
                            void (*callback)(void* data, void* user_data),
                            void* user_data);

// 反向遍历（从head的prev开始）
void dclist_foreach_backward(const DoublyCircularList* list,
                             void (*callback)(void* data, void* user_data),
                             void* user_data);

// 从指定节点开始遍历
void dclist_foreach_from_node(const DoublyNode* start_node,
                              size_t count,
                              void (*callback)(void* data, void* user_data),
                              void* user_data);
```

### 3.5 高级操作（可选实现）

```c
// 链表反转
void dclist_reverse(DoublyCircularList* list);

// 链表排序（归并排序）
void dclist_sort(DoublyCircularList* list);

// 合并两个已排序的链表
DoublyCircularList* dclist_merge_sorted(DoublyCircularList* list1,
                                        DoublyCircularList* list2);

// 分割链表
DoublyCircularList* dclist_split_at(DoublyCircularList* list, size_t index);

// 克隆链表
DoublyCircularList* dclist_clone(const DoublyCircularList* list,
                                 void* (*clone_data)(const void* data));
```

## 4. 实现重点和难点

### 4.1 关键实现点

#### 空链表处理

- 空链表时，`head == NULL`
- 插入第一个节点时，节点的 prev 和 next 都指向自己
- 删除最后一个节点时，链表变为空

#### 单节点链表处理

```c
// 单节点时的状态
node->prev == node && node->next == node
```

#### 循环特性维护

- 任何插入/删除操作都要保证循环特性
- 更新 head 指针时要特别小心

### 4.2 边界条件

1. **空链表操作**：删除、访问等操作的安全处理
2. **单节点操作**：插入删除时 prev 和 next 的正确设置
3. **索引边界**：负数索引、超出范围索引的处理
4. **内存分配失败**：malloc 失败时的错误处理

### 4.3 内存管理要点

- 每次创建节点时检查 malloc 是否成功
- 删除节点时要释放节点内存
- 如果 data 也是动态分配的，需要提供释放函数
- 避免内存泄漏和双重释放

## 5. 测试用例设计

### 5.1 基础功能测试

```c
void test_create_destroy(void);
void test_insert_operations(void);
void test_remove_operations(void);
void test_find_operations(void);
void test_traverse_operations(void);
```

### 5.2 边界情况测试

```c
void test_empty_list_operations(void);
void test_single_node_operations(void);
void test_large_list_operations(void);
void test_invalid_parameters(void);
```

### 5.3 性能测试

```c
void test_performance_insert(void);
void test_performance_delete(void);
void test_performance_search(void);
```

### 5.4 内存测试

```c
void test_memory_leaks(void);
void test_allocation_failure(void);
```

## 6. 应用场景

### 实际应用

1. **音乐播放器**：循环播放列表
2. **操作系统**：进程调度的时间片轮转
3. **游戏开发**：玩家回合制游戏
4. **缓存系统**：LRU 缓存的基础结构
5. **文本编辑器**：撤销/重做功能的历史记录

### 后续数据结构基础

- **栈和队列**：可以基于双向循环链表实现
- **哈希表**：用作冲突解决的链式法
- **图的邻接表**：存储图的边信息

## 7. 性能特点

| 操作           | 时间复杂度 | 说明               |
| -------------- | ---------- | ------------------ |
| 插入头部/尾部  | O(1)       | 直接操作 head 节点 |
| 删除头部/尾部  | O(1)       | 直接操作 head 节点 |
| 删除已知节点   | O(1)       | 双向指针的优势     |
| 查找数据       | O(n)       | 需要遍历链表       |
| 按索引访问     | O(n)       | 需要遍历到指定位置 |
| 插入到指定位置 | O(n)       | 需要先遍历到位置   |

## 8. 实现建议

### 开发顺序

1. 先实现基础结构定义
2. 实现节点的创建和销毁
3. 实现插入操作（从简单到复杂）
4. 实现删除操作
5. 实现查找和遍历
6. 添加完整的测试用例
7. 优化和调试

### 调试技巧

- 实现一个打印链表的函数，方便调试
- 检查链表完整性的函数，验证循环结构
- 使用 valgrind 检查内存泄漏
- 逐步测试，每实现一个功能就测试一遍

记住：双向循环链表是后续很多数据结构的基础，把这个实现好了，后面的学习会轻松很多！
