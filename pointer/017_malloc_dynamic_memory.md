# 动态内存分配专题（malloc 专题）

## 学习目标

- 理解 `malloc`、`free` 的用法与内存分配流程
- 能使用 `malloc` 动态创建一维数组、字符串
- 学会分配和释放二维数组
- 熟悉动态字符串数组（`char **`）的构建与释放

## 1. malloc / free 的基本语法

```c
#include <stdlib.h>

int *arr = (int *)malloc(10 * sizeof(int));  // 分配 10 个 int
if (arr == NULL) {
    printf("内存分配失败\n");
}

free(arr);  // 释放内存
```

`malloc` 只分配，不初始化内容，`calloc` 分配并置零。

## 2. malloc 一维数组

```c
int n = 5;
int *nums = (int *)malloc(n * sizeof(int));
for (int i = 0; i < n; i++) nums[i] = i + 1;
```

## 3. malloc 字符串

```c
char *str = (char *)malloc(100 * sizeof(char));
strcpy(str, "HELLO");
```

使用结束后:

```c
free(str);
```

## 4. malloc 字符串数组 (`char **`)

```c
int n = 3;
char **seqs = (char **)malloc(n * sizeof(char *));
for (int i = 0; i < n; i++) {
    seq[i] = (char *)malloc(100 * sizeof(char));
}
```

填充数据:

```c
strcpy(seqs[0], "ATCG");
strcpy(seqs[1], "GGTA");
strcpy(seqs[2], "CCTA");
```

释放内存:

```c
for (int i = 0; i < n; i++) free(seqs[i]);
free(seqs);
```

## 5. malloc 二维整型数组 (`int **`)

```c
int rows = 3, cols = 4;
int **matrix = (int **)malloc(rows * sizeof(int *));
for (int i = 0; i < rows; i++) {
    matrix[i] = (int *)malloc(cols * sizeof(int));
}
```

释放内存:

```c
for (int i = 0; i < rows; i++) free(matrix[i]);
free(matrix);
```

## 6. 练习 (ChatGPT)

### 输入若干 DNA 序列并存储

目标:

```c
char **read_dna(int n);
```

输入：序列个数 `n`，每行输入一个 DNA 序列
输出：返回一个 `char **`，内部每个 `char *` 是一条字符串
要求：

- 使用 `malloc` 构建
- 使用 `scanf("%s", seqs[i]);` 读入
- 写一个 `void print_dna(char **seqs, int n);` 输出所有序列
- 使用 `free` 释放所有内存

## QA

### 使用 malloc 的格式是什么？为什么要类型转换？可以不转吗？可以用普通类型吗？malloc 返回值是什么？

```c
int *arr = (int *)malloc(10 * sizeof(int));
```

解释：

- `malloc` 返回 `void*`，是“通用指针”，可以指向任何类型。
- 在 **C 中**，`void*` 可以自动转换为任何类型指针，所以类型转换可省略；
- 在 **C++ 中**，必须强制类型转换。

可以省略类型转换（仅限 C）：

```c
int *arr = malloc(10 * sizeof(int)); // 合法
```

不能用普通类型接收：

```c
int x = (int)malloc(...); // ❌ 错误，不要强转为 int 类型
```

---

### 为什么二维数组释放时不能直接 free(matrix); 而需要逐行释放？

如果你这样分配了二维数组：

```c
int **matrix = malloc(rows * sizeof(int *));
for (int i = 0; i < rows; i++) {
    matrix[i] = malloc(cols * sizeof(int));
}
```

你就需要这样释放：

```c
for (int i = 0; i < rows; i++) {
    free(matrix[i]); // 释放每一行
}
free(matrix); // 最后释放指针数组本身
```

原因：

- 每一行 `matrix[i]` 都是独立调用 `malloc` 分配的；
- 直接 `free(matrix);` 只释放最外层指针，其他行不会被释放；
- 这会造成 **内存泄漏**。

---

### malloc(n \* sizeof(char \*)) 中 sizeof(char \*) 的意义？

`char *` 是指针类型，`sizeof(char *)` 表示一个 “指针变量所占的字节数”。

一般在现代系统（如 64 位 Linux/macOS）中，所有指针类型（`int *`、`char *`、`float *`...）大小一样，通常为 8 字节（64 位）。

在 32 位系统中，指针大小是 4 字节（32 位）。

✅ 所以 sizeof(char \*) 就是系统中“一个指针的大小”，无论它指向什么类型。

---

### 练习题中动态分配内存

```c
char *seq = malloc(10);
int capacity = 10;
int len = 0;
char c;

while ((c = getchar()) != '\n' && c != EOF) {
    if (len + 1 >= capacity) {
        capacity *= 2;
        seq = realloc(seq, capacity);
    }
    seq[len++] = c;
}
seq[len] = '\0';
```
