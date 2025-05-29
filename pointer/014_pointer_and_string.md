# 指针与字符串（Pointer & String）

## 学习目标

- 理解 C 中字符串的存储方式
- 理解 `char *` 与字符数组的关系
- 掌握使用指针遍历和修改字符串
- 实现简单字符串处理函数

## 1. 字符串在 C 中的本质

C 中没有专门的字符串类型，**字符串本质上是以 `'\0'` 结尾的字符数组**。

```c
char s[] = "ATCG";   // 实际为 {'A', 'T', 'C', 'G', '\0'}
char *p = s;         // 指针指向首字符
```

或者

```c
char *s = "ATCG";  // 指针指向字符串常量，不能修改内容（只读）
```

## 2. 指针遍历字符串

```c
char *s = "ATCG";
while(*s != '\0') {  // 不可以是 "\0" 只能是 '\0'
    printf("%c ", *s);
    s++;  // 移动到下一个字符地址
}
```

## 常见字符串操作

| 函数     | 功能           |
| :------- | :------------- |
| `strlen` | 计算字符串长度 |
| `strcpy` | 拷贝字符串     |
| `strcmp` | 比较字符串     |
| `strcat` | 拼接字符串     |

## 练习 (来自 ChatGPT)

给定一个 DNA 序列，例如 "ATCGTAGC"，用指针实现一个函数，把它转换为互补序列并输出：

- "A ↔ T"
- "C ↔ G"
  要求:
- 用 `char *p` 遍历 `seq`
- 原地修改 (即对字符串数组操作)
- 忽略非法字符

### 拓展

- 写一个函数：统计 DNA 序列中 A/T/C/G 的个数
- 用指针实现一个类似 Python 中 `reverse()` 的函数
- 实现 `strlen`: 用指针数到 `'\0'` 为止

## QA

### "\0" 和 '\0'有什么区别

这涉及到 c 中字符和字符串的定义，使用 `''` 定义字符，使用 `""` 定义字符串，因此
`'\0'` 表示字符串的最后一个字符, 而 `"\0"` 表示一个空字符串。

### 在写 `reverse()` 函数时遇到如下错误:

```c
char *reverse(char s[])
{
    char *p = s;
    int s_len = my_strlen(s);
    char reversed_s[s_len];
    int pos = 0;
    while (*p != '\0')
    {
        reversed_s[pos] = s[s_len - pos - 1];
        p++;
    }
    reversed_s[s_len] = '\0';
    return reversed_s;
}
```

报错

```bash
014_pointer_and_string.c: In function 'reverse':
014_pointer_and_string.c:82:12: warning: function returns address of local variable [-Wreturn-local-addr]
   82 |     return reversed_s;
      |            ^~~~~~~~~~
```

原因: 这段代码会导致未定义行为，因为 reversed_s 是栈上的局部数组，函数返回后它就失效了。  
解决办法: 使用**静态数组**或者**动态内存分配（malloc）**。

```c
// 静态数组
static char reversed_s[100]; // 假设最长为 100

// 动态内存分配
char *reversed_s = (char *)malloc((s_len + 1) * sizeof(char));
```

- `malloc` 会在 **堆区** 申请内存 (先不深入了解，等到学到内存时再继续了解)
- 其不会随着函数结束而销毁
