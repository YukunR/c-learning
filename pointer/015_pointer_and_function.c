#include <stdio.h>

void swap_str(char **a, char **b)
{
    char *tmp = *a;
    *a = *b;
    *b = tmp;
}

int main()
{
    char *x = "ATP";
    char *y = "GTP";
    swap_str(&x, &y);
    printf("交换后x为 %s, y为 %s", x, y);
}