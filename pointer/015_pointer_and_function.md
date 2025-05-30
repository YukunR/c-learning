# 指针与函数

## 学习目标

- 理解为什么要通过指针传参
- 掌握“引用传递”技巧：通过函数修改外部变量
- 能写出 swap（交换变量）、array modify（修改数组元素）的函数

## 1. C 语言函数参数传递方式

### C 是**值传递**语言

```c
void set(int x) {
    x = 100;
}

int main() {
    int a = 5;
    set(a);
    printf("%d", a);  // 输出仍为 5
}
```

`x = 100` 只是改了 `x` 的副本，**a 不受影响**。

## 2. 使用指针实现 "引用传递"

```c
void set(int *x) {
    *x = 100;
}

int main() {
    int a = 5;
    set(&a);
    printf("%d\n", a);  // 100
    return 0;
}
```

`*x = 100` 表示 "把 x 指向的地址的值改为 100"。

## 3. 经典案例：swap 交换两个变量

```c
void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
```

## 4.修改数组 (字符串元素)

数组传入函数时，会退化为指针:

```c
void double_all(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] *= 2;
    }
}
```

## 5. 练习 (来自 chatGPT)

写一个两个字符串交换的函数

## QA

### `*p` 为指针，那 `p` 是什么

```c
int main() {
    int x = 1;
    int *p = &x;

    printf("x 的值是：%d\n", x);    // 输出 1
    printf("*p 的值是：%d\n", *p);  // 输出 1
    printf("p 的地址是：%p\n", (void*)p);   // 输出 &x 的地址
    printf("&x 的地址是：%p\n", (void*)&x); // 与上面一致
}
```
