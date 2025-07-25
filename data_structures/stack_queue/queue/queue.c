// queue.c

#include <stdio.h>
#include <stdlib.h>
#include "../../doubly_circular_list/doubly_circular_list.h"
#include "queue.h"

struct Queue
{
    DoublyCircularList *list;
};

Queue *queue_create(void)
{
    Queue *queue = malloc(sizeof(Queue));
    if (!queue)
    {
        fprintf(stderr, "Failed to allocate memory for Queue\n");
        return NULL;
    }
    queue->list = list_create(NULL);
    if (!queue->list)
    {
        fprintf(stderr, "Failed to create underlying list for Queue\n");
        free(queue);
        return NULL;
    }

    return queue;
}

void queue_destroy(Queue **queue)
{
    if (!queue || !*queue)
    {
        return;
    }

    list_destroy(&(*queue)->list);
    free(*queue);
    *queue = NULL;
}

bool queue_enqueue(Queue *queue, void *data)
{
    if (!queue)
    {
        fprintf(stderr, "Queue doesn't exist\n");
        return false;
    }

    list_insert_tail(queue->list, data);
    return true;
}

void *queue_dequeue(Queue *queue)
{
    if (!queue)
    {
        fprintf(stderr, "Queue doesn't exist\n");
        return NULL;
    }

    void *data = list_get_head(queue->list);
    list_remove_head(queue->list);
    return data;
}

void *queue_front(Queue *queue)
{
    if (!queue)
    {
        fprintf(stderr, "Queue doesn't exist\n");
        return NULL;
    }

    return list_get_head(queue->list);
}

void *queue_rear(Queue *queue)
{
    if (!queue)
    {
        fprintf(stderr, "Queue doesn't exist\n");
        return NULL;
    }

    return list_get_tail(queue->list);
}

bool queue_is_empty(Queue *queue)
{
    if (!queue)
    {
        fprintf(stderr, "Queue doesn't exist\n");
        return true;
    }

    return list_is_empty(queue->list);
}

size_t queue_size(Queue *queue)
{
    if (!queue)
    {
        fprintf(stderr, "Queue doesn't exist\n");
        return 0;
    }

    return list_size(queue->list);
}

void queue_print(Queue *queue, void (*print_func)(const void *data))
{
    if (!queue)
    {
        fprintf(stderr, "Queue doesn't exist\n");
        return;
    }
    list_print_forward(queue->list, print_func);
}

void queue_clear(Queue *queue) {
    if (!queue) return;

    list_clear(queue->list);
}