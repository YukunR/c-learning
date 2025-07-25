// queue.h
#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct Queue Queue;

// 创建和销毁
Queue *queue_create(void);
void queue_destroy(Queue **queue);

// 队列操作
bool queue_enqueue(Queue *queue, void *data);
void *queue_dequeue(Queue *queue);
void *queue_front(Queue *queue);
void *queue_rear(Queue *queue);

// 状态查询
bool queue_is_empty(Queue *queue);
size_t queue_size(Queue *queue);

// 调试和工具
void queue_print(Queue *queue, void (*print_func)(const void *data));
void queue_clear(Queue *queue);

#endif