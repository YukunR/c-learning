/*
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
*/

#include <stdio.h>
#include <stdlib.h>

char *reverse(char s[]);
int my_strlen(char s[]);
char *complement_dna(char s[]);

int main()
{
    char seq[] = "ATCGTAGC";
    char *rev = reverse(seq);
    printf("%s\n", complement_dna(seq));
    printf("%d\n", my_strlen(seq));
    printf("%s\n", rev);
    free(rev);

    return 0;
}

int my_strlen(char s[])
{
    int len = 0;
    char *p = s;
    while (*p != '\0')
    {
        len++;
        p++;
    }
    return len;
}

char *complement_dna(char s[])
{
    char *p = s;
    while (*p != '\0')
    {
        if (*p == 'A') *p = 'T';
        else if (*p == 'T') *p = 'A';
        else if (*p == 'G') *p = 'C';
        else if (*p == 'C') *p = 'G';
        p++;
    }
    return s;
}

char *reverse(char s[])
{
    char *p = s;
    int s_len = my_strlen(s);
    char *reversed_s = (char *)malloc((s_len + 1) * sizeof(char));
    if (reversed_s == NULL)
    {
        printf("内存分配失败\n");
        return NULL;
    }
    int pos = 0;
    while (*p != '\0')
    {
        reversed_s[pos] = s[s_len - pos - 1];
        pos++;
        p++;
    }

    reversed_s[s_len] = '\0';
    return reversed_s;
}