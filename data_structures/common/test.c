#include "common.h"
#include <stdio.h>


int main() {
    int result = compare_int(&(int){2}, &(int){2});
    printf("%d", result);
}
