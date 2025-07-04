// test.c

#include <assert.h>
#include <stdio.h>
#include "doubly_circular_list.h"
#include "../commen/commen.h"

int main()
{
    printf("=== Testing Doubly Circular Linked List ===\n");

    // Create the list
    DoublyCircularList *list = list_create(compare_int);
    if (!list)
    {
        fprintf(stderr, "Failed to create list\n");
        return 1;
    }
    printf("is empty: %d\n", list_is_empty(list));

    // Insert elements at head
    int *a = malloc(sizeof(int));
    *a = 10;
    int *b = malloc(sizeof(int));
    *b = 20;
    list_insert_head(list, a); // [10]
    list_insert_head(list, b); // [20, 10]

    // Insert elements at tail
    int *c = malloc(sizeof(int));
    *c = 30;
    int *d = malloc(sizeof(int));
    *d = 40;
    list_insert_tail(list, c); // [20, 10, 30]
    list_insert_tail(list, d); // [20, 10, 30, 40]

    // Insert at specific index
    int *e = malloc(sizeof(int));
    *e = 25;
    list_insert_at(list, 2, e); // [20, 10, 25, 30, 40]

    // Print forward
    printf("Forward: ");
    list_print_forward(list, print_int);
    printf("\n");

    // Print backward
    printf("Backward: ");
    list_print_backward(list, print_int);
    printf("\n");

    // Access values
    printf("Head value: %d\n", *(int *)list_get_head(list));
    printf("Tail value: %d\n", *(int *)list_get_tail(list));
    printf("Value at index 2: %d\n", *(int *)list_get_at(list, 2));

    // Remove elements
    list_remove_head(list);  // remove 20
    list_remove_tail(list);  // remove 40
    list_remove_at(list, 1); // remove 25

    printf("After removal (forward): ");
    list_print_forward(list, print_int); // should print [10, 30]
    printf("\n");

    // Find elements
    printf("%d\n", list_size(list));
    printf("Find 30 at index: %d\n", list_find(list, &(int){30}));
    printf("Find 15 at index: %d\n", list_find(list, &(int){15}));
    printf("Contain 10?: %d\n", list_contains(list, &(int){10}));

    printf("is empty: %d\n", list_is_empty(list));
    // Destroy the list
    list_destroy(&list);
    if (!list)
    {
        printf("List successfully destroyed.\n");
    }

    return 0;
}