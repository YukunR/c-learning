# 哈希表实现指南

## 概述

哈希表（Hash Table）是一种通过哈希函数将键（key）映射到表中位置来访问记录的数据结构。它的核心思想是用空间换时间，实现平均情况下 O(1) 的插入、删除和查找操作。

**基本工作流程：**

```
key → hash_function(key) → index → bucket[index] → value
```

**与已有数据结构的关系：**

- **动态数组**：作为桶（bucket）的容器
- **双向循环链表**：解决哈希冲突（链地址法）

## 基本概念

### 哈希函数（Hash Function）

哈希函数将任意长度的输入（键）转换为固定长度的输出（哈希值/索引）：

```c
size_t hash = hash_function(key) % table_size;
```

**好的哈希函数特性：**

1. **确定性**：相同输入总是产生相同输出
2. **高效性**：计算速度快
3. **均匀分布**：尽可能将键均匀分布到所有桶中
4. **雪崩效应**：输入的小变化导致输出的大变化

### 负载因子（Load Factor）

```
负载因子 = 元素个数 / 桶的数量
```

- **过低（< 0.5）**：浪费空间
- **适中（0.7-0.75）**：性能最佳 ⭐
- **过高（> 0.8）**：冲突增加，性能下降

## 核心操作

| 操作       | 描述                 | 平均时间复杂度 | 最坏时间复杂度 |
| ---------- | -------------------- | -------------- | -------------- |
| `insert`   | 插入键值对           | O(1)           | O(n)           |
| `search`   | 根据键查找值         | O(1)           | O(n)           |
| `delete`   | 删除键值对           | O(1)           | O(n)           |
| `update`   | 更新键对应的值       | O(1)           | O(n)           |
| `contains` | 检查键是否存在       | O(1)           | O(n)           |
| `size`     | 获取哈希表中元素个数 | O(1)           | O(1)           |

## 哈希函数设计（从简单到复杂）

### 第一层：通用基础哈希函数

所有复杂哈希算法的基础，用于处理任意字节序列：

```c
// 推荐：FNV-1a 作为通用基础哈希函数
size_t simple_hash(const void *key, size_t key_size) {
    const unsigned char *bytes = (const unsigned char *)key;
    size_t hash = 2166136261u;  // FNV offset basis

    for (size_t i = 0; i < key_size; i++) {
        hash ^= bytes[i];
        hash *= 16777619u;  // FNV prime
    }
    return hash;
}
```

**为什么选择 FNV-1a？**

- 分布均匀，雪崩效应好
- 对不同长度的键都表现良好
- 实现简单，性能优秀

### 第二层：散列方法

基于通用哈希函数，将哈希值映射到表索引：

**1. 除法散列法（推荐入门）**

```c
size_t hash_division(const void *key, size_t key_size, size_t table_size) {
    return simple_hash(key, key_size) % table_size;
}
```

- ✅ **优点**：计算快速，实现简单
- ❌ **缺点**：表大小选择影响分布
- 📋 **建议**：表大小使用质数，避免 2 的幂

**2. 乘法散列法（进阶）**

```c
size_t hash_multiplication(const void *key, size_t key_size, size_t table_size) {
    size_t key_hash = simple_hash(key, key_size);
    // 使用整数运算模拟乘法散列（避免浮点运算）
    uint64_t hash_val = (uint64_t)key_hash * 2654435769u;  // 黄金比例 * 2^32
    return (size_t)((hash_val >> 32) * table_size >> 32);
}
```

- ✅ **优点**：对表大小不敏感，分布更均匀
- ❌ **缺点**：计算稍复杂

### 第三层：专用字符串哈希算法

**1. DJB2 哈希（入门推荐）**

```c
size_t hash_string_djb2(const char *str) {
    size_t hash = 5381;  // 经过测试的质数
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}
```

**算法解析：**

- **乘数 33**：`(hash << 5) + hash` 等价于 `hash * 33`
- **为什么是 33？** `33 = 32 + 1 = 2^5 + 1`，可用位运算优化
- **简单高效**：易理解，性能好

**计算示例：**

```
字符串 "Hi" 的哈希计算：
初始：hash = 5381
处理 'H' (72)：hash = 5381 * 33 + 72 = 177645
处理 'i' (105)：hash = 177645 * 33 + 105 = 5862390
```

**2. FNV-1a 字符串版（推荐生产）**

```c
size_t hash_string_fnv1a(const char *str) {
    size_t hash = 2166136261u;    // FNV offset basis
    while (*str) {
        hash ^= (unsigned char)*str++;  // 先异或
        hash *= 16777619;               // 再乘 FNV prime
    }
    return hash;
}
```

- ✅ **分布优秀**：统计测试表现最佳
- ✅ **雪崩效应强**：小变化导致大差异

### 第四层：高性能哈希算法

**MurmurHash3（工业级）**

```c
// 简化版 MurmurHash3
uint32_t murmur3_32(const void *key, size_t len, uint32_t seed) {
    const uint8_t *data = (const uint8_t*)key;
    const int nblocks = len / 4;
    uint32_t h1 = seed;

    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    // 块处理：一次处理4字节
    const uint32_t *blocks = (const uint32_t *)(data + nblocks*4);
    for(int i = -nblocks; i; i++) {
        uint32_t k1 = blocks[i];
        k1 *= c1;
        k1 = (k1 << 15) | (k1 >> 17);  // 位旋转
        k1 *= c2;

        h1 ^= k1;
        h1 = (h1 << 13) | (h1 >> 19);
        h1 = h1*5 + 0xe6546b64;
    }

    // 处理剩余字节...
    // [省略尾部处理和最终混合代码]

    return h1;
}
```

**特点：**

- **块处理**：提高处理效率
- **多重混合**：位旋转 + 乘法 + 异或
- **统计优秀**：通过各种随机性测试

### 算法选择指南

| 使用场景          | 推荐算法     | 理由               |
| ----------------- | ------------ | ------------------ |
| 🎓 **学习入门**   | DJB2         | 简单易懂，容易实现 |
| 🏢 **一般项目**   | FNV-1a       | 平衡性能和分布质量 |
| ⚡ **高性能需求** | MurmurHash3  | 速度快，分布优秀   |
| 🔧 **嵌入式系统** | 简单除法散列 | 内存和计算资源有限 |

## 冲突解决策略

### 1. 链地址法（Chaining）

**实现思路：**

- 每个桶存储一个双向循环链表的头指针
- 冲突时，将新元素插入到对应桶的链表中
- 查找时，遍历对应桶的链表进行比较

```
buckets[0] → NULL
buckets[1] → [Node1] ⇄ [Node2] ⇄ [Node3] (双向循环链表)
buckets[2] → [Node4]
buckets[3] → NULL
...
```

**优缺点：**

- ✅ 实现简单，支持动态扩容
- ✅ 删除操作容易实现
- ✅ 对负载因子不敏感
- ❌ 需要额外的指针空间
- ❌ 缓存局部性较差

### 2. 开放地址法（Open Addressing）

#### 2.1 线性探测法（Linear Probing）

**实现思路：**

- 发生冲突时，按顺序查找下一个空位置
- 探测序列：h(k), h(k)+1, h(k)+2, ..., h(k)+i (mod m)
- 删除时需要标记为"已删除"状态

```
索引: 0    1    2    3    4    5    6    7
值:  NULL  A   NULL  B    C   NULL NULL NULL

插入D，h(D)=1，发生冲突:
步骤: 1(占用) → 2(空闲) → 插入成功
```

**优缺点：**

- ✅ 缓存局部性好，空间利用率高
- ✅ 实现相对简单
- ❌ 容易产生聚集现象（Primary Clustering）
- ❌ 删除操作复杂（需要重新组织）

#### 2.2 二次探测法（Quadratic Probing）

**实现思路：**

- 探测序列：h(k), h(k)+1², h(k)+2², ..., h(k)+i² (mod m)
- 减少线性探测的聚集问题
- 要求表长度为质数且负载因子 ≤ 0.5

```
插入D，h(D)=1，发生冲突:
尝试位置: 1 → 1+1² → 1+2² → 1+3² → ...
即:      1 → 2    → 5    → 10   → ... (mod m)
```

**优缺点：**

- ✅ 减少一次聚集现象
- ✅ 保持较好的缓存性能
- ❌ 可能产生二次聚集（Secondary Clustering）
- ❌ 对表长度和负载因子有严格要求

#### 2.3 双散列法（Double Hashing）

**实现思路：**

- 使用两个散列函数：h₁(k) 和 h₂(k)
- 探测序列：h₁(k), h₁(k)+h₂(k), h₁(k)+2·h₂(k), ...
- h₂(k) 必须与表长度 m 互质

```c
h₁(k) = k % m
h₂(k) = 7 - (k % 7)  // 确保与 m 互质
```

```
插入D，h₁(D)=1，h₂(D)=3，发生冲突:
尝试位置: 1 → 1+3 → 1+6 → 1+9 → ...
即:      1 → 4   → 7   → 10  → ... (mod m)
```

**优缺点：**

- ✅ 最大程度避免聚集现象
- ✅ 探测序列均匀分布
- ✅ 理论性能最佳
- ❌ 计算开销较大（两个哈希函数）
- ❌ 实现相对复杂

### 性能对比总结

| 方法         | 平均查找时间 | 最坏查找时间 | 空间开销 | 删除复杂度 | 推荐场景     |
| ------------ | ------------ | ------------ | -------- | ---------- | ------------ |
| **链地址法** | O(1+α)       | O(n)         | 高       | 简单       | 通用，易扩展 |
| **线性探测** | O(1/(1-α))   | O(n)         | 低       | 复杂       | 内存敏感     |
| **二次探测** | O(1/(1-α))   | O(n)         | 低       | 复杂       | 平衡性能     |
| **双散列**   | O(1/(1-α))   | O(n)         | 低       | 复杂       | 高性能要求   |

_注：α 为负载因子_

## 扩容机制

### 何时扩容？

```c
if (table->size >= table->capacity * table->max_load_factor) {
    hashtable_resize(table, table->capacity * 2);
}
```

### 扩容步骤

1. **创建新的更大的桶数组**（通常扩大 2 倍）
2. **重新哈希所有元素**（rehashing）
3. **释放旧的桶数组**

### 扩容优化策略

- **渐进式扩容**：分多次完成，避免长时间阻塞
- **双表技术**：维护新旧两个表，逐步迁移
- **负载监控**：提前预警，避免突发扩容

## 应用场景

### 实际应用

1. **数据库索引**：B+树 + 哈希索引的组合
2. **缓存系统**：Redis、Memcached 的核心数据结构
3. **编译器**：符号表管理，变量名到地址的映射
4. **Python 字典**：dict 类型的底层实现
5. **文件系统**：目录项缓存
6. **网络路由**：IP 地址到下一跳的映射
7. **去重算法**：快速检测重复元素

### 算法中的应用

```c
// 示例：统计字符串中字符出现次数
void count_characters(const char *str) {
    HashTable *counter = hashtable_create(26, hash_string_djb2,
                                         strcmp, free, free);

    for (int i = 0; str[i]; i++) {
        char key[2] = {str[i], '\0'};
        int *count = hashtable_search_string(counter, key);

        if (count) {
            (*count)++;
        } else {
            int *new_count = malloc(sizeof(int));
            *new_count = 1;
            hashtable_insert_string(counter, key, new_count);
        }
    }

    hashtable_destroy(&counter);
}
```

## 实现建议

### 学习路径

```
第一阶段：基础实现
├── 1. 实现简单的除法散列
├── 2. 使用链地址法处理冲突
├── 3. 支持字符串键的基本操作
└── 4. 添加简单的扩容机制

第二阶段：功能完善
├── 1. 添加多种哈希函数选择
├── 2. 支持泛型键（void*）
├── 3. 实现开放地址法
└── 4. 添加统计和调试功能

第三阶段：性能优化
├── 1. 实现高性能哈希算法
├── 2. 优化内存管理
├── 3. 添加渐进式扩容
└── 4. 性能测试和调优
```

### 测试策略

```c
// 测试用例设计
void test_basic_operations(void);      // 基本增删改查
void test_collision_handling(void);    // 冲突处理测试
void test_resize_operations(void);     // 扩容机制测试
void test_edge_cases(void);           // 边界条件测试
void test_memory_management(void);     // 内存泄漏检测
void test_performance(void);          // 性能基准测试

// 性能测试示例
void benchmark_hash_functions(void) {
    const char *test_strings[] = {"apple", "banana", "cherry", ...};

    // 测试不同哈希函数的分布均匀性
    for (each hash_function) {
        measure_distribution(hash_function, test_strings);
        measure_collision_rate(hash_function, test_strings);
        measure_computation_time(hash_function, test_strings);
    }
}
```

### 常见陷阱和解决方案

1. **内存泄漏**

   ```c
   // ❌ 错误：忘记释放键和值
   hashtable_delete(table, key, key_size);

   // ✅ 正确：使用析构函数
   hashtable_destroy(&table);  // 自动调用 key_destroy 和 value_destroy
   ```

2. **哈希函数选择不当**

   ```c
   // ❌ 错误：简单累加（分布不均）
   size_t bad_hash(const char *str) {
       size_t sum = 0;
       while (*str) sum += *str++;
       return sum;
   }

   // ✅ 正确：使用成熟算法
   size_t good_hash(const char *str) {
       return hash_string_djb2(str);
   }
   ```

3. **负载因子控制**
   ```c
   // ✅ 及时扩容
   if (load_factor > 0.75) {
       hashtable_resize(table, table->capacity * 2);
   }
   ```

## 总结

哈希表是一个**平衡艺术**的数据结构：

- **时间 vs 空间**：用额外空间换取时间效率
- **简单 vs 复杂**：简单哈希函数 vs 复杂但分布更好的算法
- **通用 vs 专用**：通用实现 vs 针对特定数据优化

**学习重点：**

1. **理解核心思想**：空间换时间的权衡
2. **掌握实现技巧**：冲突处理、扩容机制
3. **关注性能指标**：负载因子、冲突率、分布均匀性
4. **培养设计思维**：如何根据应用场景选择合适的策略

通过实现哈希表，你将深入理解如何将多种数据结构有机组合，这为学习更复杂的数据结构（如 B+ 树、LSM 树等）奠定坚实基础。
