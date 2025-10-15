#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"
#include "../../common/common.h"

void test_queue_fifo_property() {
    printf("=== test_queue_fifo_property ===\n");

    Queue *queue = queue_create();
    assert(queue != NULL);

    int a = 10, b = 20, c = 30;
    assert(queue_enqueue(queue, &a));
    assert(queue_enqueue(queue, &b));
    assert(queue_enqueue(queue, &c));

    assert(queue_size(queue) == 3);
    queue_print(queue, print_int);

    int *val = (int *)queue_dequeue(queue);
    assert(*val == 10);

    val = (int *)queue_dequeue(queue);
    assert(*val == 20);

    val = (int *)queue_dequeue(queue);
    assert(*val == 30);

    assert(queue_is_empty(queue));

    queue_destroy(&queue);
    printf("✅ Passed\n\n");
}

void test_queue_empty_operations() {
    printf("=== test_queue_empty_operations ===\n");

    Queue *queue = queue_create();
    assert(queue != NULL);

    assert(queue_is_empty(queue));
    assert(queue_size(queue) == 0);
    assert(queue_front(queue) == NULL);
    assert(queue_rear(queue) == NULL);
    assert(queue_dequeue(queue) == NULL);

    queue_destroy(&queue);
    printf("✅ Passed\n\n");
}

void test_queue_large_data() {
    printf("=== test_queue_large_data ===\n");

    Queue *queue = queue_create();
    assert(queue != NULL);

    int n = 10000;
    int *data = malloc(sizeof(int) * n);
    assert(data != NULL);

    for (int i = 0; i < n; ++i) {
        data[i] = i;
        assert(queue_enqueue(queue, &data[i]));
    }

    for (int i = 0; i < n; ++i) {
        int *val = (int *)queue_dequeue(queue);
        assert(*val == i);
    }

    assert(queue_is_empty(queue));
    free(data);
    queue_destroy(&queue);
    printf("✅ Passed\n\n");
}

void test_queue_memory_management() {
    printf("=== test_queue_memory_management ===\n");

    Queue *queue = queue_create();
    assert(queue != NULL);

    int *x = malloc(sizeof(int));
    int *y = malloc(sizeof(int));
    int *z = malloc(sizeof(int));
    assert(x && y && z);

    *x = 100;
    *y = 200;
    *z = 300;

    queue_enqueue(queue, x);
    queue_enqueue(queue, y);
    queue_enqueue(queue, z);

    queue_clear(queue);  // 清空队列结构，但不释放 data 内存
    assert(queue_is_empty(queue));

    // 手动释放你自己分配的内存
    free(x);
    free(y);
    free(z);

    queue_destroy(&queue);
    printf("✅ Passed\n\n");
}

int main() {
    test_queue_fifo_property();
    test_queue_empty_operations();
    test_queue_large_data();
    test_queue_memory_management();
    return 0;
}