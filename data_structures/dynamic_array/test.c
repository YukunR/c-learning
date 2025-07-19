#include "dynamic_array.h"
#include "../commen/commen.h"
#include <assert.h>

void test_dynamic_array_init_destroy()
{
    printf("=== 测试 DynamicArray 创建与销毁 ===\n");

    // 创建数组，传入初始容量为 0（应自动设为默认容量 2）
    DynamicArray *arr = array_create(0);
    assert(arr != NULL);

    // 初始 size 应该为 0
    assert(arr->size == 0);

    // 初始 capacity 应该为默认值（你设为 2）
    assert(arr->capacity >= 2);

    // 销毁数组
    array_destroy(&arr);
    assert(arr == NULL); // 确保销毁后指针被置为 NULL

    printf("✅ 创建与销毁测试通过\n\n");
}

void test_dynamic_array_push_pop()
{
    printf("=== 测试 push_back 与 pop_back ===\n");

    DynamicArray *arr = array_create(2);
    assert(arr != NULL);

    int nums[5] = {10, 20, 30, 40, 50};

    // 插入元素
    for (int i = 0; i < 5; i++)
    {
        bool ok = array_push_back(arr, &nums[i]);
        assert(ok);
        assert(arr->size == i + 1);
    }

    // 检查扩容是否发生
    assert(arr->capacity == 8);

    // 逆序弹出元素并验证值
    for (int i = 4; i >= 1; i--)
    {
        void *data = array_pop_back(arr);
        assert(data == &nums[i]); // 地址对上
        assert(arr->size == i);   // size 减少
    }

    assert(arr->capacity == 4);

    array_pop_back(arr);
    assert(arr->capacity == 2);

    // 再 pop 应该为空
    void *data = array_pop_back(arr);
    assert(data == NULL);

    array_destroy(&arr);
    printf("✅ push_back 与 pop_back 测试通过\n\n");
}

void test_dynamic_array_get_set()
{
    printf("=== 测试 get_at 与 set_at ===\n");

    DynamicArray *arr = array_create(4);
    assert(arr != NULL);

    int nums[4] = {100, 200, 300, 400};
    for (int i = 0; i < 4; i++)
    {
        bool ok = array_push_back(arr, &nums[i]);
        assert(ok);
    }

    // 正确 get 测试
    for (int i = 0; i < 4; i++)
    {
        void *ptr = array_get_at(arr, i);
        assert(ptr == &nums[i]); // 地址应一致
    }

    // 越界 get 测试
    void *out = array_get_at(arr, 4);
    assert(out == NULL);
    out = array_get_at(arr, 999);
    assert(out == NULL);

    // 设置新值
    int x = 1234;
    bool ok = array_set_at(arr, 2, &x);
    assert(ok);
    assert((int *)array_get_at(arr, 2) == &x);

    // 越界 set 测试
    ok = array_set_at(arr, 100, &x);
    assert(!ok);

    // NULL array 测试
    assert(array_get_at(NULL, 0) == NULL);
    assert(array_set_at(NULL, 0, &x) == false);

    array_destroy(&arr);
    printf("✅ get_at 与 set_at 测试通过\n\n");
}

void test_insert_and_remove_at()
{
    printf("=== 测试 insert_at 与 remove_at ===\n");

    DynamicArray *arr = array_create(2);
    assert(arr != NULL);

    int *nums[6];
    for (int i = 0; i < 6; i++)
    {
        nums[i] = malloc(sizeof(int));
        *nums[i] = i * 10; // 0, 10, 20, ...
    }

    printf("测试尾插\n");
    // 插入尾部：模拟 push_back
    for (int i = 0; i < 3; i++)
    {
        assert(array_insert_at(arr, i, nums[i]));
        assert(array_size(arr) == i + 1);
    }

    // 此时数组为：[0, 10, 20]

    printf("测试头插\n");
    // 插入头部
    assert(array_insert_at(arr, 0, nums[3])); // 插入30 -> [30, 0, 10, 20]
    assert(array_size(arr) == 4);
    assert(*(int *)array_get_at(arr, 0) == 30);

    printf("测试插入中间\n");
    // 插入中间
    assert(array_insert_at(arr, 2, nums[4])); // 插入40 -> [30, 0, 40, 10, 20]
    assert(array_size(arr) == 5);
    assert(*(int *)array_get_at(arr, 2) == 40);

    printf("测试尾插\n");
    // 插入尾部
    assert(array_insert_at(arr, array_size(arr), nums[5])); // 插入50 -> [..., 50]
    assert(array_size(arr) == 6);
    assert(*(int *)array_get_at(arr, 5) == 50);
    array_print(arr, print_int);

    printf("测试移除中间\n");
    // 移除中间元素
    array_remove_at(arr, 2); // 移除 40
    assert(array_size(arr) == 5);
    array_print(arr, print_int);

    printf("测试移除头部\n");
    // 移除头部
    array_remove_at(arr, 0); // 移除 30
    assert(array_size(arr) == 4);
    array_print(arr, print_int);

    printf("测试移除尾部\n");
    // 移除尾部
    array_remove_at(arr, array_size(arr) - 1); // 移除 50
    assert(array_size(arr) == 3);

    array_print(arr, print_int);
    // 此时应该剩下：[0, 10, 20]
    assert(*(int *)array_get_at(arr, 0) == 0);
    assert(*(int *)array_get_at(arr, 1) == 10);
    assert(*(int *)array_get_at(arr, 2) == 20);

    // 释放剩余元素
    for (size_t i = 0; i < array_size(arr); i++)
    {
        free(array_get_at(arr, i));
    }

    printf("destroy\n");
    array_destroy(&arr);
    printf("destroy done\n");
    assert(arr == NULL);

    printf("✅ insert_at 与 remove_at 测试通过\n\n");
}

void test_find_and_contain()
{
    printf("=== 测试 find 与 contain ===\n");

    DynamicArray *arr = array_create(4);
    assert(arr != NULL);

    int nums[4] = {100, 200, 300, 100};
    for (int i = 0; i < 4; i++)
    {
        bool ok = array_push_back(arr, &nums[i]);
        assert(ok);
    }

    array_print(arr, print_int);
    assert(array_find(arr, &(int){100}, compare_int) == 0);
    assert(array_find(arr, &(int){200}, compare_int) == 1);
    assert(array_find(arr, &(int){1}, compare_int) == -1);

    assert(array_contains(arr, &(int){100}, compare_int));
    assert(!array_contains(arr, &(int){1}, compare_int));

    array_contains(NULL, &(int){100}, compare_int);
    array_contains(arr, &(int){100}, NULL);

    array_destroy(&arr);
    printf("✅ find 与 contain 测试通过\n\n");
}

void test_clear_reverse_clone()
{
    printf("=== 测试 clear / reverse / clone ===\n");

    DynamicArray *arr = array_create(5);
    assert(arr != NULL);

    int nums[5] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++)
    {
        bool ok = array_push_back(arr, &nums[i]);
        assert(ok);
    }

    array_print(arr, print_int);  // 打印原数组 [1, 2, 3, 4, 5]

    // 测试 reverse
    array_reverse(arr);
    array_print(arr, print_int);  // 应该是 [5, 4, 3, 2, 1]
    assert(*(int *)arr->data[0] == 5);
    assert(*(int *)arr->data[4] == 1);

    // 测试 clone（浅拷贝）
    DynamicArray *clone = array_clone(arr, NULL);
    assert(clone != NULL);
    assert(clone->size == arr->size);
    for (int i = 0; i < clone->size; i++)
    {
        assert(clone->data[i] == arr->data[i]);  // 浅拷贝应该是地址一致
    }

    array_print(clone, print_int);  // 应该和 arr 相同

    // 测试 clear
    array_clear(arr);
    assert(arr->size == 0);
    for (int i = 0; i < arr->capacity; i++)
    {
        assert(arr->data[i] == NULL);  // 所有 data 应设为 NULL
    }

    array_print(arr, print_int);  // 应该打印 []

    array_destroy(&arr);
    array_destroy(&clone);
    printf("✅ clear / reverse / clone 测试通过\n\n");
}

void test_array_clone()
{
    printf("=== 测试 array_clone（浅拷贝 & 深拷贝）===\n");

    DynamicArray *arr = array_create(4);
    assert(arr != NULL);

    int *nums[4];
    for (int i = 0; i < 4; i++)
    {
        nums[i] = malloc(sizeof(int));
        *nums[i] = (i + 1) * 10;
        bool ok = array_push_back(arr, nums[i]);
        assert(ok);
    }

    array_print(arr, print_int);  // [10 20 30 40]

    // --- 浅拷贝 ---
    DynamicArray *shallow = array_clone(arr, NULL);
    assert(shallow->size == arr->size);
    for (int i = 0; i < arr->size; i++)
    {
        assert(shallow->data[i] == arr->data[i]);  // 地址一致
    }
    array_print(shallow, print_int);

    // --- 深拷贝 ---
    DynamicArray *deep = array_clone(arr, clone_int);
    assert(deep->size == arr->size);
    for (int i = 0; i < arr->size; i++)
    {
        assert(deep->data[i] != arr->data[i]);  // 地址不同
        assert(*(int*)deep->data[i] == *(int*)arr->data[i]);  // 内容相同
    }
    array_print(deep, print_int);

    // 修改原数组，验证深拷贝不受影响
    *(int*)arr->data[0] = 999;
    assert(*(int*)deep->data[0] != *(int*)arr->data[0]);

    printf("原数组: ");
    array_print(arr, print_int);
    printf("深拷贝: ");
    array_print(deep, print_int);
    printf("浅拷贝: ");
    array_print(shallow, print_int);

    // 释放资源
    for (int i = 0; i < arr->size; i++) free(nums[i]);  // 原始值
    for (int i = 0; i < deep->size; i++) free(deep->data[i]);  // 深拷贝值

    array_destroy(&arr);
    array_destroy(&shallow);
    array_destroy(&deep);

    printf("✅ array_clone 测试通过\n\n");
}

int main()
{
    test_array_clone();
    return 0;
}