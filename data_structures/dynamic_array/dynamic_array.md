# 动态数组（Dynamic Array）学习指南

## 什么是动态数组

动态数组就像一个可以自动变大的"盒子"，你可以往里面放东西，当盒子满了，它会自动换一个更大的盒子。这是学习数据结构的重要基石。

## 核心思想

想象你在整理书架：

- **固定数组**：就像一个固定大小的书架，满了就放不下了
- **动态数组**：就像一个魔法书架，快满时会自动扩大一倍

## 三个关键概念

### 1. 大小（size）vs 容量（capacity）

```
[数据][数据][数据][空位][空位][空位]
  ↑__size=3__↑     ↑__预留空间__↑
  ↑_________capacity=6_________↑
```

### 2. 扩容时机

```
添加前: [A][B][C][_]  size=3, capacity=4
添加D:  [A][B][C][D]  size=4, capacity=4  ← 满了！
扩容后: [A][B][C][D][_][_][_][_]  size=4, capacity=8
```

### 3. 为什么要预留空间？

- 避免每次添加元素都要重新分配内存
- 2 倍扩容保证添加元素的平均时间复杂度是 O(1)

## 实现要点

### 1. 内存管理核心

```c
// 扩容操作的关键逻辑
if (array->size == array->capacity) {
    size_t new_capacity = array->capacity * GROWTH_FACTOR;
    void* new_data = realloc(array->data, new_capacity * sizeof(void*));
    if (!new_data) return DA_ERROR_MEMORY;
    array->data = new_data;
    array->capacity = new_capacity;
}
```

### 2. 错误处理策略

- **内存分配失败**：返回错误码，保持原数据结构不变
- **索引越界**：参数验证，防止访问非法内存
- **空指针检查**：对所有输入参数进行验证

### 3. 通用性设计

- 使用 `void*` 存储任意类型数据
- 提供用户自定义的比较函数
- 支持用户自定义的数据释放函数

## 扩容复杂度分析

### Q：为什么扩容后添加元素的平均复杂度是 O(1)？

乍看起来，扩容需要复制所有元素，时间复杂度是 O(n)，但为什么说平均复杂度是 O(1)呢？

### 摊还分析（Amortized Analysis）

关键在于**摊还分析**：虽然某次操作很昂贵，但这种昂贵的操作很少发生。

#### 具体推导过程

假设我们从容量为 1 开始，每次扩容 2 倍，连续插入 n 个元素：

**扩容发生的时机：**

- 第 1 次扩容：size=1 时，从容量 1→2，复制 1 个元素
- 第 2 次扩容：size=2 时，从容量 2→4，复制 2 个元素
- 第 3 次扩容：size=4 时，从容量 4→8，复制 4 个元素
- 第 k 次扩容：size=2^(k-1)时，复制 2^(k-1)个元素

#### 总成本计算

插入 n 个元素的总成本 = 普通插入成本 + 扩容成本

```
普通插入成本：n × 1 = n

扩容成本：1 + 2 + 4 + 8 + ... + 2^(k-1)
其中k满足：2^(k-1) ≤ n < 2^k

这是一个几何级数：
1 + 2 + 4 + ... + 2^(k-1) = 2^k - 1 < 2n

总成本 = n + (2n-1) = 3n - 1
```

#### 平均复杂度

```
平均每次插入的成本 = (3n-1)/n ≈ 3 = O(1)
```

### 为什么必须是 2 倍扩容？

#### 1 倍扩容的问题

如果每次扩容只增加固定数量（比如+1）：

```
容量变化：1→2→3→4→...→n
扩容次数：n-1次
总扩容成本：1+2+3+...+(n-1) = n(n-1)/2 = O(n²)
平均复杂度：O(n²)/n = O(n)  ❌
```

#### 1.5 倍扩容可以吗？

可以！任何大于 1 的常数倍扩容都能保证 O(1)平均复杂度：

```
1.5倍扩容的成本：
1 + 1.5 + 2.25 + 3.375 + ... < 常数×n
平均复杂度仍然是O(1)  ✅
```

#### 为什么选择 2 倍？

- **简单**：位运算实现，`capacity << 1`
- **平衡**：内存浪费适中（最多浪费 50%）
- **标准**：大多数标准库的选择

### 直观理解

想象你在搬家：

```
第1次搬家：1个箱子 → 付出1单位努力，获得1个位置
第2次搬家：2个箱子 → 付出2单位努力，获得2个位置
第3次搬家：4个箱子 → 付出4单位努力，获得4个位置
...
```

每次搬家的"性价比"是固定的：付出 k 单位努力，获得 k 个新位置。

### 代码中的体现

```c
bool array_push_back(DynamicArray* array, void* data) {
    // 触发扩容的条件
    if (array->size == array->capacity) {
        // 关键：2倍扩容保证平均O(1)
        size_t new_capacity = array->capacity * 2;

        // 这次操作是O(n)，但很少发生
        void** new_data = realloc(array->data,
                                 new_capacity * sizeof(void*));

        array->data = new_data;
        array->capacity = new_capacity;
    }

    // 这次操作是O(1)，经常发生
    array->data[array->size] = data;
    array->size++;
    return true;
}
```

## 防止抖动（Thrashing）问题

### 什么是抖动？

如果扩容和缩容的阈值设置不当，会导致频繁的内存重新分配：

```text
假设容量为8，size为4的数组，如果缩容阈值是50%：

插入一个元素：size=5，不需要扩容
删除一个元素：size=4，触发缩容到4
插入一个元素：size=5，触发扩容到8
删除一个元素：size=4，触发缩容到4
...无限循环，每次操作都要重新分配内存！
```

### 解决方案：使用不同的扩容和缩容阈值

```c
// 扩容：当size达到capacity时
if (array->size == array->capacity) {
    new_capacity = array->capacity * 2;  // 扩容到2倍
}

// 缩容：当size小于capacity的1/4时
if (array->size < array->capacity / 4 && array->capacity > INITIAL_CAPACITY) {
    new_capacity = array->capacity / 2;  // 缩容到1/2
}
```

### 为什么 1/4 是安全的？

```
容量变化过程：
1. 容量为8，size从4降到2时缩容到4
2. 现在容量为4，size=2
3. 要再次触发缩容，需要size<1，即size=0
4. 要触发扩容，需要size=4
5. 在size从2到4的过程中，不会触发任何容量调整

这样就避免了频繁的内存重新分配
```

### 实际代码示例

```c
bool array_push_back(DynamicArray* array, void* data) {
    // 扩容检查
    if (array->size == array->capacity) {
        if (!resize_array(array, array->capacity * 2)) {
            return false;
        }
    }

    array->data[array->size++] = data;
    return true;
}

void* array_pop_back(DynamicArray* array) {
    if (array->size == 0) return NULL;

    void* data = array->data[--array->size];

    // 缩容检查：size < capacity/4 且 capacity > 最小值
    if (array->size < array->capacity / 4 &&
        array->capacity > INITIAL_CAPACITY) {
        resize_array(array, array->capacity / 2);
    }

    return data;
}
```

### 实际例子

连续插入 8 个元素的过程：

```
初始：capacity=1, size=0  []

插入1: 扩容1→2, 复制0个元素, 总cost=1  [1,_]
插入2: 扩容2→4, 复制1个元素, 总cost=2  [1,2,_,_]
插入3: 普通插入, 总cost=1              [1,2,3,_]
插入4: 扩容4→8, 复制3个元素, 总cost=4  [1,2,3,4,_,_,_,_]
插入5: 普通插入, 总cost=1              [1,2,3,4,5,_,_,_]
插入6: 普通插入, 总cost=1              [1,2,3,4,5,6,_,_]
插入7: 普通插入, 总cost=1              [1,2,3,4,5,6,7,_]
插入8: 普通插入, 总cost=1              [1,2,3,4,5,6,7,8]

总成本：1+2+1+4+1+1+1+1 = 12
平均成本：12/8 = 1.5 = O(1) ✅
```

## 学习重点

1. **理解扩容**：什么时候扩容，怎么扩容
2. **掌握 realloc**：C 语言内存管理的关键函数
3. **注意边界条件**：空数组、满数组的处理
4. **内存安全**：每个 malloc 都要有对应的 free

## 建议的实现顺序

1. **第一步**：实现创建和销毁
2. **第二步**：实现尾部添加（重点是扩容逻辑）
3. **第三步**：实现通过索引访问
4. **第四步**：实现尾部删除
5. **第五步**：实现任意位置插入和删除
