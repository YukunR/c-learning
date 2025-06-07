/*
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

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LEN 1000

char **read_dna(int n);
void print_dna(char **seqs, int n);

int main()
{
    int n;
    printf("请输入需要输入的 DNA 序列个数:");
    scanf("%d", &n);
    char **dnas = read_dna(n);
    print_dna(dnas, n);
    for (int i = 0; i < n; i++)
    {
        free(dnas[i]); // 释放每个字符串
    }
    free(dnas); // 释放字符串指针数组
}

char **read_dna(int n)
{
    char **dnas = (char **)malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++)
    {
        char *buf = (char *)malloc(MAX_LEN * sizeof(char));
        printf("请输入 DNA 序列 %d: ", i + 1);
        scanf("%s", buf);
        dnas[i] = buf;
    }
    return dnas;
}

void print_dna(char **seqs, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%s\n", seqs[i]);
    }
}