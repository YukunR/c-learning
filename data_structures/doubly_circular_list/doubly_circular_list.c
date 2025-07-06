/*
Implement of linked list.

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "doubly_circular_list.h"
#include "../commen/commen.h"

struct Node
{
    void *data;
    struct Node *prev;
    struct Node *next;
};

struct DoublyCircularList
{
    Node *head;
    size_t length;
    int (*compare)(const void *a, const void *b);
};

// init and destroy
DoublyCircularList *list_create(int (*compare)(const void *a, const void *b))
{
    DoublyCircularList *dcl = malloc(sizeof(DoublyCircularList));
    if (dcl == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for LinkedList\n");
    }
    // set list head as a sentinel node
    Node *head = malloc(sizeof(Node));
    if (head == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for LinkedList head\n");
    }
    head->data = NULL;
    head->prev = head;
    head->next = head;

    dcl->head = head;
    dcl->compare = compare;
    dcl->length = 0;
}

void list_destroy(DoublyCircularList **list)
{
    if (!list)
    {
        fprintf(stderr, "List doesn't exist\n");
        return;
    }
    Node *head = (*list)->head;
    Node *cur = head->next;
    Node *next;
    while (cur != head)
    {
        next = cur->next;
        free(cur);
        cur = next;
    }

    free(head);
    free(*list);
    *list = NULL;
}

Node *init_node(void *data)
{
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for Node\n");
    }
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;

    return new_node;
}

// insert
bool list_insert_head(DoublyCircularList *list, void *data)
{
    if (!list)
    {
        fprintf(stderr, "List doesn't exist\n");
        return false;
    };

    Node *head = list->head;
    Node *first = head->next;
    // init new node
    Node *new_node = init_node(data);

    new_node->next = first;
    new_node->prev = head;

    head->next = new_node;
    first->prev = new_node;

    list->length += 1;
    return true;
}

bool list_insert_tail(DoublyCircularList *list, void *data)
{
    if (!list)
    {
        fprintf(stderr, "List doesn't exist\n");
        return false;
    };

    Node *head = list->head;
    Node *tail = head->prev;

    Node *new_node = init_node(data);

    new_node->next = head;
    new_node->prev = tail;
    head->prev = new_node;
    tail->next = new_node;

    list->length += 1;
    return true;
}

bool list_insert_at(DoublyCircularList *list, size_t index, void *data)
{
    /**
     * Insert a new node before the node at the specified index.
     * The index is zero-based; index 0 inserts before the first element.
     */
    if (!list)
    {
        fprintf(stderr, "List doesn't exist\n");
        return false;
    };
    if (index > list->length || index < 0)
    {
        fprintf(stderr, "Index %d out of bounds [0, %d]\n", index, list->length);
        return false;
    }

    Node *new_node = init_node(data);

    // get node at index
    Node *target_node = list_get_node_at(list, index);
    if (!target_node)
    {
        return false;
    }
    Node *prev_node = target_node->prev;

    // update chain
    new_node->prev = prev_node;
    new_node->next = target_node;
    prev_node->next = new_node;
    target_node->prev = new_node;

    list->length += 1;
    return true;
}

// remove functions
bool list_remove_head(DoublyCircularList *list)
{
    if (!list)
    {
        fprintf(stderr, "List doesn't exist\n");
        return false;
    };

    Node *head = list->head;
    Node *first = list->head->next;
    Node *second = first->next;

    if (first == head)
    {
        fprintf(stderr, "List is empty. Nothing to remove\n");
        return false;
    }

    head->next = second;
    second->prev = head;

    free(first);
    list->length -= 1;
    return true;
}

bool list_remove_tail(DoublyCircularList *list)
{
    if (!list)
    {
        fprintf(stderr, "List doesn't exist\n");
        return false;
    };

    Node *head = list->head;
    Node *last = list->head->prev;
    Node *penultimate = last->prev;

    if (last == head)
    {
        fprintf(stderr, "List is empty. Nothing to remove\n");
        return false;
    }

    head->prev = penultimate;
    penultimate->next = head;

    free(last);
    list->length -= 1;
    return true;
}

bool list_remove_at(DoublyCircularList *list, size_t index)
{
    if (!list)
    {
        fprintf(stderr, "List doesn't exist\n");
        return false;
    };
    if (index >= list->length)
    {
        fprintf(stderr, "Index %d out of bounds [0, %d)\n", index, list->length);
        return false;
    }
    if (list->length == 1)
    {
        fprintf(stderr, "List is empty. Nothing to remove\n");
        return false;
    }

    // get node at index
    Node *target_node = list_get_node_at(list, index);
    if (!target_node)
    {
        return false;
    }
    Node *prev_node = target_node->prev;
    Node *next_node = target_node->next;

    // update chain
    prev_node->next = next_node;
    next_node->prev = prev_node;

    free(target_node);
    list->length -= 1;
    return true;
}

// get functions
Node *list_get_node_at(DoublyCircularList *list, size_t index)
{
    if (!list)
    {
        fprintf(stderr, "List doesn't exist\n");
        return NULL;
    };
    if (index > list->length || index < 0)
    {
        fprintf(stderr, "Index %d out of bounds [0, %d)\n", index, list->length);
        return NULL;
    }
    if (index == list->length)
    {
        return list->head;
    }

    /**
     * If it's in the first half of the list, search forward;
     * otherwise, search backward.
     */
    Node *cur = NULL;
    if (index <= list->length / 2)
    {
        // search forward
        cur = list->head->next;
        for (int i = 0; i < index; i++)
        {
            cur = cur->next;
        }
    }
    else
    {
        // search backward
        cur = list->head->prev;
        for (int i = 0; i < list->length - index - 1; i++)
        {
            cur = cur->prev;
        }
    }

    return cur;
}

void *list_get_head(DoublyCircularList *list)
{
    if (!list)
    {
        fprintf(stderr, "List doesn't exist\n");
        return NULL;
    }
    return (void *)list->head->next->data;
}

void *list_get_tail(DoublyCircularList *list)
{
    if (!list)
    {
        fprintf(stderr, "List doesn't exist\n");
        return NULL;
    }
    return (void *)list->head->prev->data;
}

void *list_get_at(DoublyCircularList *list, size_t index)
{
    if (!list)
    {
        fprintf(stderr, "List doesn't exist\n");
        return NULL;
    }
    if (index > list->length || index < 0)
    {
        fprintf(stderr, "Index %d out of bounds [0, %d]\n", index, list->length);
        return false;
    }

    Node *target_node = list_get_node_at(list, index);
    if (!target_node)
    {
        return NULL;
    }
    return (void *)target_node->data;
}

// set
bool list_set_at(DoublyCircularList *list, size_t index, void *data)
{
    if (!list)
    {
        fprintf(stderr, "List doesn't exist\n");
        return false;
    }
    if (index >= list->length || index < 0)
    {
        fprintf(stderr, "Index %d out of bounds [0, %d)\n", index, list->length);
        return false;
    }

    Node *target_node = list_get_node_at(list, index);
    if (!target_node)
    {
        return false;
    }
    target_node->data = data;
    return true;
}

// find
int list_find(DoublyCircularList *list, const void *data)
{
    /**
     * Find the index of the first node whose data matches given `data`
     * return -1 if not found
     */
    if (!list)
    {
        fprintf(stderr, "List doesn't exist\n");
        return -1;
    }

    int index = -1;
    Node *cur = list->head->next;
    for (int i = 0; i < list->length; i++)
    {
        if (list->compare(data, cur->data) == 0)
        {

            index = i;
            break;
        }
        cur = cur->next;
    }
    return index;
}

bool list_contains(DoublyCircularList *list, const void *data)
{
    if (!list)
    {
        fprintf(stderr, "List doesn't exist\n");
        return -1;
    }

    Node *cur = list->head->next;
    for (int i = 0; i < list->length; i++)
    {
        if (list->compare(data, cur->data) == 0)
        {

            return true;
        }
        cur = cur->next;
    }
    return false;
}

// list info
size_t list_size(DoublyCircularList *list)
{
    if (!list)
    {
        fprintf(stderr, "List doesn't exist\n");
        return -1;
    }
    return list->length;
}

bool list_is_empty(DoublyCircularList *list)
{
    if (!list)
    {
        fprintf(stderr, "List doesn't exist\n");
        return false;
    }
    return list->length == 0;
}

// list operation
void list_reverse(DoublyCircularList *list)
{
    if (!list)
    {
        fprintf(stderr, "List doesn't exist\n");
        return;
    }

    Node *cur = list->head;
    for (int i = 0; i < list->length + 1; i++)
    {
        Node *temp = cur->next;
        cur->next = cur->prev;
        cur->prev = temp;
        cur = temp;
    }
}

DoublyCircularList *list_clone(DoublyCircularList *list, void *(*clone_data)(const void *data))
{
    if (!list)
    {
        fprintf(stderr, "List doesn't exist\n");
        return NULL;
    }

    DoublyCircularList *new_list = list_create(list->compare);
    // copy old list
    Node *cur = list->head->next;
    for (int i = 0; i < list->length; i++)
    {
        list_insert_tail(new_list, clone_data ? clone_data(cur->data) : cur->data);
        cur = cur->next;
    }

    return new_list;
}

// print functions
void list_print_forward(DoublyCircularList *list, void (*print_func)(const void *data))
{
    if (!list)
    {
        fprintf(stderr, "List doesn't exist\n");
        return;
    };

    Node *cur = list->head->next;
    while (cur != list->head)
    {
        print_func(cur->data);
        cur = cur->next;
    }
    printf("\n");
}

void list_print_backward(DoublyCircularList *list, void (*print_func)(const void *data))
{
    if (!list)
    {
        fprintf(stderr, "List doesn't exist\n");
        return;
    };

    Node *cur = list->head->prev;
    while (cur != list->head)
    {
        print_func(cur->data);
        cur = cur->prev;
    }
    printf("\n");
}
