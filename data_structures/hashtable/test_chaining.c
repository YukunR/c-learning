/**
 * test_chaining.c - Test cases specifically for chaining hashtable implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hashtable.h"
#include "hashtable_chaining.h"
#include "hash.h"
#include "../common/common.h"

void print_separator(const char *test_suite_name) {
    printf("\n--- %s ---\n", test_suite_name);
}

void test_assert(int condition, const char *test_name) {
    if (condition) {
        printf("✓ %s\n", test_name);
    } else {
        printf("✗ %s FAILED\n", test_name);
        exit(1);
    }
}

void test_chaining_basic_operations() {
    print_separator("Chaining: Basic Operations");
    
    HashKeyOps keyops = {
        .hash = hash_fnv1a,
        .eq = compare_int,
        .destroy_key = NULL,
        .destroy_val = NULL
    };
    
    HashTable *ht = hashtable_chaining_create(4, 0.75, keyops);
    test_assert(ht != NULL, "Create chaining hashtable");
    test_assert(hashtable_size(ht) == 0, "Initial size is 0");
    test_assert(hashtable_capacity(ht) == 4, "Initial capacity is 4");
    
    // Test insertion
    int key1 = 10, val1 = 100;
    int key2 = 20, val2 = 200;
    int key3 = 30, val3 = 300;
    
    test_assert(hashtable_insert(ht, &key1, sizeof(int), &val1), "Insert key1");
    test_assert(hashtable_insert(ht, &key2, sizeof(int), &val2), "Insert key2");
    test_assert(hashtable_insert(ht, &key3, sizeof(int), &val3), "Insert key3");
    test_assert(hashtable_size(ht) == 3, "Size after insertions");
    
    // Test search
    int *found = (int*)hashtable_search(ht, &key1, sizeof(int));
    test_assert(found != NULL && *found == val1, "Search existing key1");
    
    found = (int*)hashtable_search(ht, &key2, sizeof(int));
    test_assert(found != NULL && *found == val2, "Search existing key2");
    
    int nonexistent = 999;
    found = (int*)hashtable_search(ht, &nonexistent, sizeof(int));
    test_assert(found == NULL, "Search non-existent key");
    
    // Test update
    int new_val1 = 1000;
    test_assert(hashtable_update(ht, &key1, sizeof(int), &new_val1), "Update existing key");
    found = (int*)hashtable_search(ht, &key1, sizeof(int));
    test_assert(found != NULL && *found == new_val1, "Verify updated value");
    
    // Test deletion
    test_assert(hashtable_delete(ht, &key2, sizeof(int)), "Delete existing key");
    test_assert(hashtable_size(ht) == 2, "Size after deletion");
    found = (int*)hashtable_search(ht, &key2, sizeof(int));
    test_assert(found == NULL, "Verify deleted key not found");
    
    hashtable_destroy(&ht);
    test_assert(ht == NULL, "Destroy hashtable");
}

void test_chaining_collision_handling() {
    print_separator("Chaining: Collision Handling");
    
    HashKeyOps keyops = {
        .hash = hash_fnv1a,
        .eq = compare_int,
        .destroy_key = NULL,
        .destroy_val = NULL
    };
    
    // Create a very small hashtable to force many collisions
    HashTable *ht = hashtable_chaining_create(2, 2.0, keyops); // High load factor
    test_assert(ht != NULL, "Create small hashtable for collisions");
    
    // Insert keys that will definitely collide in a size-2 table
    int keys[] = {1, 3, 5, 7, 9, 11, 13, 15}; // All odd numbers
    int vals[] = {10, 30, 50, 70, 90, 110, 130, 150};
    size_t num_keys = sizeof(keys) / sizeof(keys[0]);
    
    for (size_t i = 0; i < num_keys; i++) {
        test_assert(hashtable_insert(ht, &keys[i], sizeof(int), &vals[i]),
                   "Insert key causing collisions");
    }
    
    test_assert(hashtable_size(ht) == num_keys, "All keys inserted despite collisions");
    
    // Verify all keys can be found (chaining should handle collisions)
    for (size_t i = 0; i < num_keys; i++) {
        int *found = (int*)hashtable_search(ht, &keys[i], sizeof(int));
        test_assert(found != NULL && (int)*found == vals[i], "Find collided key");
    }
    
    // Check collision statistics
    HashStats stats = hashtable_get_stats(ht);
    printf("Collision statistics:\n");
    printf("  - Total elements: %zu\n", stats.total_elements);
    printf("  - Collision count: %zu\n", stats.collision_count);
    printf("  - Max chain length: %zu\n", stats.max_chain_or_probe);
    printf("  - Average chain length: %.2f\n", stats.average_chain_length);
    
    test_assert(stats.collision_count > 0, "Collisions were detected");
    test_assert(stats.max_chain_or_probe > 1, "Chain length > 1 due to collisions");
    
    hashtable_destroy(&ht);
}

void test_chaining_rehashing() {
    print_separator("Chaining: Rehashing Behavior");
    
    HashKeyOps keyops = {
        .hash = hash_fnv1a,
        .eq = compare_int,
        .destroy_key = NULL,
        .destroy_val = NULL
    };
    
    // Start with small capacity and low load factor to trigger rehashing
    HashTable *ht = hashtable_chaining_create(4, 0.5, keyops);
    size_t initial_capacity = hashtable_capacity(ht);
    
    printf("Initial capacity: %zu\n", initial_capacity);
    
    // Insert enough elements to trigger rehashing
    int keys[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int vals[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    
    for (int i = 0; i < 10; i++) {
        size_t old_capacity = hashtable_capacity(ht);
        hashtable_insert(ht, &keys[i], sizeof(int), &vals[i]);
        size_t new_capacity = hashtable_capacity(ht);
        
        if (new_capacity > old_capacity) {
            printf("Rehashing triggered: capacity %zu -> %zu after inserting key %d\n",
                   old_capacity, new_capacity, keys[i]);
        }
    }
    
    size_t final_capacity = hashtable_capacity(ht);
    test_assert(final_capacity > initial_capacity, "Capacity increased due to rehashing");
    test_assert(hashtable_size(ht) == 10, "All elements preserved after rehashing");
    
    // Verify all keys are still accessible after rehashing
    for (int i = 0; i < 10; i++) {
        int *found = (int*)hashtable_search(ht, &keys[i], sizeof(int));
        test_assert(found != NULL && *found == vals[i], "Key accessible after rehashing");
    }
    
    printf("Final capacity: %zu\n", final_capacity);
    
    hashtable_destroy(&ht);
}

void test_chaining_string_keys() {
    print_separator("Chaining: String Key Operations");
    
    HashKeyOps keyops = {
        .hash = hash_fnv1a,
        .eq = compare_string,
        .destroy_key = NULL,  // Static strings
        .destroy_val = NULL   // Static strings
    };
    
    HashTable *ht = hashtable_chaining_create(8, 0.75, keyops);
    test_assert(ht != NULL, "Create hashtable for strings");
    
    // Test string operations using helper functions
    struct {
        char *name;
        char *age;
    } people[] = {
        {"Alice", "25"},
        {"Bob", "30"},
        {"Charlie", "35"},
        {"Diana", "28"},
        {"Eve", "22"}
    };
    
    size_t num_people = sizeof(people) / sizeof(people[0]);
    
    // Insert all people
    for (size_t i = 0; i < num_people; i++) {
        test_assert(hashtable_insert_string(ht, people[i].name, people[i].age),
                   "Insert string key-value pair");
    }
    
    test_assert(hashtable_size(ht) == num_people, "All string pairs inserted");
    
    // Search for all people
    for (size_t i = 0; i < num_people; i++) {
        char *found = (char*)hashtable_search_string(ht, people[i].name);
        test_assert(found != NULL && strcmp(found, people[i].age) == 0,
                   "Find string key");
    }
    
    // Test deletion
    test_assert(hashtable_delete_string(ht, "Charlie"), "Delete string key");
    test_assert(hashtable_size(ht) == num_people - 1, "Size after string deletion");
    test_assert(hashtable_search_string(ht, "Charlie") == NULL, "Deleted string key not found");
    
    hashtable_destroy(&ht);
}

void test_chaining_edge_cases() {
    print_separator("Chaining: Edge Cases");
    
    HashKeyOps keyops = {
        .hash = hash_fnv1a,
        .eq = compare_int,
        .destroy_key = NULL,
        .destroy_val = NULL
    };
    
    HashTable *ht = hashtable_chaining_create(4, 0.75, keyops);
    
    // Test operations on empty hashtable
    int key = 42;
    test_assert(hashtable_search(ht, &key, sizeof(int)) == NULL, 
               "Search in empty hashtable");
    test_assert(!hashtable_delete(ht, &key, sizeof(int)), 
               "Delete from empty hashtable");
    test_assert(!hashtable_update(ht, &key, sizeof(int), &key), 
               "Update in empty hashtable");
    
    // Test duplicate key insertion (should update, not add new)
    int val1 = 100, val2 = 200;
    test_assert(hashtable_insert(ht, &key, sizeof(int), &val1), 
               "Insert initial value");
    size_t size_after_first = hashtable_size(ht);
    
    test_assert(hashtable_insert(ht, &key, sizeof(int), &val2), 
               "Insert duplicate key (should update)");
    test_assert(hashtable_size(ht) == size_after_first, 
               "Size unchanged after duplicate insertion");
    
    int *found = (int*)hashtable_search(ht, &key, sizeof(int));
    test_assert(found != NULL && *found == val2, "Value updated for duplicate key");
    
    // Test with zero initial capacity (should default to minimum)
    hashtable_destroy(&ht);
    ht = hashtable_chaining_create(0, 0.75, keyops);
    test_assert(ht != NULL, "Create hashtable with 0 initial capacity");
    test_assert(hashtable_capacity(ht) >= 8, "Capacity defaults to minimum");
    
    hashtable_destroy(&ht);
}

int main() {
    printf("Chaining Hashtable Implementation Tests\n");
    printf("=====================================\n");
    
    test_chaining_basic_operations();
    test_chaining_collision_handling();
    test_chaining_rehashing();
    test_chaining_string_keys();
    test_chaining_edge_cases();
    
    printf("\n✓ All chaining hashtable tests passed!\n");
    printf("Chaining implementation appears to be working correctly.\n");
    
    return 0;
}