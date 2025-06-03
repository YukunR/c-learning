/*
你有以下序列：

```c
char *seqs[3] = {
    "ATCG",
    "GGTA",
    "CCTA"
};
```

写一个函数 `print_seqs(char *seqs[], int n)`，用指针方式输出所有序列内容。

### 拓展建议（选做）

- 用二维数组 `char seqs[3][10]` 存储 DNA 序列，并尝试交换其中两行
- 使用指针打印任意维数组（例如 `int ***data`）
*/

#include <stdio.h>

void print_seqs(char *seqs[], int n);
void swap_rows(char **seqs, int row1, int row2);

int main()
{
    char *seqs[3] = {
        "ATCG",
        "GGTA",
        "CCTA"};

    print_seqs(seqs, 3);
    swap_rows(seqs, 1, 2);
    print_seqs(seqs, 3);
    return 0;
}

void print_seqs(char *seqs[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%s\n", seqs[i]);
    }
}

void swap_rows(char **seqs, int row1, int row2)
{
    char *tmp = seqs[row1];
    seqs[row1] = seqs[row2];
    seqs[row2] = tmp;
}