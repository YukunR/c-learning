/**
 * test_oa.c - Test cases for open addressing hashtable implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hashtable.h"
#include "hashtable_oa.h"
#include "hash.h"
#include "../common/common.h"

void print_separator(const char *test_suite_name)
{
    printf("\n--- %s ---\n", test_suite_name);
}

void test_assert(int condition, const char *test_name)
{
    if (condition)
    {
        printf("✓ %s\n", test_name);
    }
    else
    {
        printf("✗ %s FAILED\n", test_name);
        exit(1);
    }
}

void test_oa_basic_operations()
{
    print_separator("Open Addressing: Basic Operations");

    HashKeyOps keyops = {
        .hash = hash_fnv1a,
        .eq = compare_int,
        .destroy_key = NULL,
        .destroy_val = NULL};

    HashTable *ht = hashtable_oa_create(8, 0.5, keyops, PROBE_LINEAR, NULL);
    test_assert(ht != NULL, "Create OA hashtable with linear probing");
    test_assert(hashtable_size(ht) == 0, "Initial size is 0");
    test_assert(hashtable_capacity(ht) == 8, "Initial capacity is 8");

    // Test insertion
    int key1 = 10, val1 = 100;
    int key2 = 20, val2 = 200;
    int key3 = 30, val3 = 300;

    test_assert(hashtable_insert(ht, &key1, sizeof(int), &val1), "Insert key1");
    test_assert(hashtable_insert(ht, &key2, sizeof(int), &val2), "Insert key2");
    test_assert(hashtable_insert(ht, &key3, sizeof(int), &val3), "Insert key3");
    test_assert(hashtable_size(ht) == 3, "Size after insertions");

    // Test search
    int *found = (int *)hashtable_search(ht, &key1, sizeof(int));
    test_assert(found != NULL && *found == val1, "Search existing key1");

    found = (int *)hashtable_search(ht, &key2, sizeof(int));
    test_assert(found != NULL && *found == val2, "Search existing key2");

    int nonexistent = 999;
    found = (int *)hashtable_search(ht, &nonexistent, sizeof(int));
    test_assert(found == NULL, "Search non-existent key");

    // Test update
    int new_val1 = 1000;
    test_assert(hashtable_update(ht, &key1, sizeof(int), &new_val1), "Update existing key");
    found = (int *)hashtable_search(ht, &key1, sizeof(int));
    test_assert(found != NULL && *found == new_val1, "Verify updated value");

    // Test deletion
    test_assert(hashtable_delete(ht, &key2, sizeof(int)), "Delete existing key");
    test_assert(hashtable_size(ht) == 2, "Size after deletion");
    found = (int *)hashtable_search(ht, &key2, sizeof(int));
    test_assert(found == NULL, "Verify deleted key not found");

    hashtable_destroy(&ht);
    test_assert(ht == NULL, "Destroy hashtable");
}

void test_oa_probe_strategies()
{
    print_separator("Open Addressing: Probe Strategies");

    HashKeyOps keyops = {
        .hash = hash_fnv1a,
        .eq = compare_int,
        .destroy_key = NULL,
        .destroy_val = free};

    // Test Linear Probing
    printf("\nTesting Linear Probing:\n");
    HashTable *ht_linear = hashtable_oa_create(8, 0.7, keyops, PROBE_LINEAR, NULL);
    test_assert(ht_linear != NULL, "Create hashtable with linear probing");

    for (int i = 0; i < 5; i++)
    {
        int key = i * 10;
        int *v = malloc(sizeof *v);
        *v = i * 100;
        test_assert(hashtable_insert(ht_linear, &key, sizeof key, v),
                    "Linear probe: Insert key");
    }
    test_assert(hashtable_size(ht_linear) == 5, "Linear probe: Size check");
    hashtable_destroy(&ht_linear);

    // Test Quadratic Probing
    printf("\nTesting Quadratic Probing:\n");
    HashTable *ht_quad = hashtable_oa_create(8, 0.7, keyops, PROBE_QUADRATIC, NULL);
    test_assert(ht_quad != NULL, "Create hashtable with quadratic probing");

    for (int i = 0; i < 5; i++)
    {
        int key = i * 10;
        int *v = malloc(sizeof *v);
        *v = i * 100;
        test_assert(hashtable_insert(ht_quad, &key, sizeof key, v),
                    "Quadratic probe: Insert key");
    }
    test_assert(hashtable_size(ht_quad) == 5, "Quadratic probe: Size check");
    hashtable_destroy(&ht_quad);

    // Test Double Hashing
    printf("\nTesting Double Hashing:\n");
    HashTable *ht_double = hashtable_oa_create(8, 0.7, keyops, PROBE_DOUBLEHASH, hash_fnv1a);
    test_assert(ht_double != NULL, "Create hashtable with double hashing");

    for (int i = 0; i < 5; i++)
    {
        int key = i * 10;
        int *v = malloc(sizeof *v);
        *v = i * 100;
        test_assert(hashtable_insert(ht_double, &key, sizeof key, v),
                    "Double hash: Insert key");
    }
    test_assert(hashtable_size(ht_double) == 5, "Double hash: Size check");

    // Verify all keys can be found
    for (int i = 0; i < 5; i++)
    {
        int key = i * 10;
        int *found = (int *)hashtable_search(ht_double, &key, sizeof(int));
        test_assert(found != NULL && *found == key * 10, "Double hash: Find key");
    }

    hashtable_destroy(&ht_double);
}

void test_oa_collision_handling()
{
    print_separator("Open Addressing: Collision Handling");

    HashKeyOps keyops = {
        .hash = hash_fnv1a,
        .eq = compare_int,
        .destroy_key = NULL,
        .destroy_val = NULL};

    // Create a small hashtable to force collisions
    HashTable *ht = hashtable_oa_create(4, 0.7, keyops, PROBE_LINEAR, NULL);
    test_assert(ht != NULL, "Create small hashtable for collisions");

    // Insert keys that will cause collisions
    int keys[] = {1, 5, 9, 13, 17, 21}; // These will likely collide
    int vals[] = {10, 50, 90, 130, 170, 210};
    size_t num_keys = sizeof(keys) / sizeof(keys[0]);

    size_t inserted = 0;
    for (size_t i = 0; i < num_keys; i++)
    {
        if (hashtable_insert(ht, &keys[i], sizeof(int), &vals[i]))
        {
            inserted++;
        }
    }

    printf("Successfully inserted %zu/%zu keys\n", inserted, num_keys);
    test_assert(inserted > 0, "At least some keys inserted despite collisions");
    test_assert(hashtable_size(ht) == inserted, "Size matches inserted count");

    // Verify inserted keys can be found
    for (size_t i = 0; i < inserted; i++)
    {
        int *found = (int *)hashtable_search(ht, &keys[i], sizeof(int));
        test_assert(found != NULL && *found == vals[i], "Find collided key");
    }

    printf("Load factor: %.2f\n", hashtable_load_factor(ht));

    hashtable_destroy(&ht);
}

void test_oa_rehashing()
{
    print_separator("Open Addressing: Rehashing Behavior");

    HashKeyOps keyops = {
        .hash = hash_fnv1a,
        .eq = compare_int,
        .destroy_key = NULL,
        .destroy_val = NULL};

    // Start with small capacity and low load factor to trigger rehashing
    HashTable *ht = hashtable_oa_create(4, 0.5, keyops, PROBE_LINEAR, NULL);
    size_t initial_capacity = hashtable_capacity(ht);

    printf("Initial capacity: %zu\n", initial_capacity);

    // Insert enough elements to trigger rehashing
    int keys[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int vals[12] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120};

    for (int i = 0; i < 12; i++)
    {
        size_t old_capacity = hashtable_capacity(ht);
        double old_load = hashtable_load_factor(ht);

        hashtable_insert(ht, &keys[i], sizeof(int), &vals[i]);

        size_t new_capacity = hashtable_capacity(ht);
        double new_load = hashtable_load_factor(ht);

        if (new_capacity > old_capacity)
        {
            printf("Rehashing triggered: capacity %zu -> %zu (load: %.2f -> %.2f) after inserting key %d\n",
                   old_capacity, new_capacity, old_load, new_load, keys[i]);
        }
    }

    size_t final_capacity = hashtable_capacity(ht);
    test_assert(final_capacity > initial_capacity, "Capacity increased due to rehashing");
    test_assert(hashtable_size(ht) == 12, "All elements preserved after rehashing");

    // Verify all keys are still accessible after rehashing
    for (int i = 0; i < 12; i++)
    {
        int *found = (int *)hashtable_search(ht, &keys[i], sizeof(int));
        test_assert(found != NULL && *found == vals[i], "Key accessible after rehashing");
    }

    printf("Final capacity: %zu, Load factor: %.2f\n",
           final_capacity, hashtable_load_factor(ht));

    hashtable_destroy(&ht);
}

void test_oa_tombstone_handling()
{
    print_separator("Open Addressing: Tombstone Handling");

    HashKeyOps keyops = {
        .hash = hash_fnv1a,
        .eq = compare_int,
        .destroy_key = NULL,
        .destroy_val = NULL};

    HashTable *ht = hashtable_oa_create(8, 0.7, keyops, PROBE_LINEAR, NULL);
    test_assert(ht != NULL, "Create hashtable");

    // Insert multiple keys
    int keys[] = {10, 20, 30, 40, 50};
    int vals[] = {100, 200, 300, 400, 500};
    size_t num_keys = sizeof(keys) / sizeof(keys[0]);

    for (size_t i = 0; i < num_keys; i++)
    {
        hashtable_insert(ht, &keys[i], sizeof(int), &vals[i]);
    }

    size_t size_before_delete = hashtable_size(ht);
    printf("Size before deletions: %zu\n", size_before_delete);

    // Delete some keys (creating tombstones)
    test_assert(hashtable_delete(ht, &keys[1], sizeof(int)), "Delete key 20");
    test_assert(hashtable_delete(ht, &keys[3], sizeof(int)), "Delete key 40");

    size_t size_after_delete = hashtable_size(ht);
    printf("Size after deletions: %zu\n", size_after_delete);
    test_assert(size_after_delete == size_before_delete - 2, "Size decreased correctly");

    // Verify deleted keys are not found
    test_assert(hashtable_search(ht, &keys[1], sizeof(int)) == NULL,
                "Deleted key 20 not found");
    test_assert(hashtable_search(ht, &keys[3], sizeof(int)) == NULL,
                "Deleted key 40 not found");

    // Verify remaining keys are still accessible (probing should skip tombstones)
    int *found = (int *)hashtable_search(ht, &keys[0], sizeof(int));
    test_assert(found != NULL && *found == vals[0], "Key 10 still accessible");

    found = (int *)hashtable_search(ht, &keys[2], sizeof(int));
    test_assert(found != NULL && *found == vals[2], "Key 30 still accessible");

    found = (int *)hashtable_search(ht, &keys[4], sizeof(int));
    test_assert(found != NULL && *found == vals[4], "Key 50 still accessible");

    // Re-insert into tombstone positions
    int new_key = 25, new_val = 250;
    test_assert(hashtable_insert(ht, &new_key, sizeof(int), &new_val),
                "Insert into tombstone position");
    found = (int *)hashtable_search(ht, &new_key, sizeof(int));
    test_assert(found != NULL && *found == new_val, "New key found after tombstone reuse");

    hashtable_destroy(&ht);
}

void test_oa_string_keys()
{
    print_separator("Open Addressing: String Key Operations");

    HashKeyOps keyops = {
        .hash = hash_fnv1a,
        .eq = compare_string,
        .destroy_key = NULL, // Static strings
        .destroy_val = NULL  // Static strings
    };

    HashTable *ht = hashtable_oa_create(8, 0.6, keyops, PROBE_LINEAR, NULL);
    test_assert(ht != NULL, "Create hashtable for strings");

    // Test string operations using helper functions
    struct
    {
        char *name;
        char *age;
    } people[] = {
        {"Alice", "25"},
        {"Bob", "30"},
        {"Charlie", "35"},
        {"Diana", "28"},
        {"Eve", "22"}};

    size_t num_people = sizeof(people) / sizeof(people[0]);

    // Insert all people
    for (size_t i = 0; i < num_people; i++)
    {
        test_assert(hashtable_insert_string(ht, people[i].name, people[i].age),
                    "Insert string key-value pair");
    }

    test_assert(hashtable_size(ht) == num_people, "All string pairs inserted");

    // Search for all people
    for (size_t i = 0; i < num_people; i++)
    {
        char *found = (char *)hashtable_search_string(ht, people[i].name);
        test_assert(found != NULL && strcmp(found, people[i].age) == 0,
                    "Find string key");
    }

    // Test update
    char *new_age = "36";
    test_assert(hashtable_update(ht, "Charlie", strlen("Charlie") + 1, new_age),
                "Update string value");
    char *found = (char *)hashtable_search_string(ht, "Charlie");
    test_assert(found != NULL && strcmp(found, new_age) == 0, "Verify updated string value");

    // Test deletion
    test_assert(hashtable_delete_string(ht, "Bob"), "Delete string key");
    test_assert(hashtable_size(ht) == num_people - 1, "Size after string deletion");
    test_assert(hashtable_search_string(ht, "Bob") == NULL, "Deleted string key not found");

    hashtable_destroy(&ht);
}

void test_oa_edge_cases()
{
    print_separator("Open Addressing: Edge Cases");

    HashKeyOps keyops = {
        .hash = hash_fnv1a,
        .eq = compare_int,
        .destroy_key = NULL,
        .destroy_val = NULL};

    HashTable *ht = hashtable_oa_create(8, 0.5, keyops, PROBE_LINEAR, NULL);

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

    int *found = (int *)hashtable_search(ht, &key, sizeof(int));
    test_assert(found != NULL && *found == val2, "Value updated for duplicate key");

    // Test with zero initial capacity (should default to minimum)
    hashtable_destroy(&ht);
    ht = hashtable_oa_create(0, 0.5, keyops, PROBE_LINEAR, NULL);
    test_assert(ht != NULL, "Create hashtable with 0 initial capacity");
    test_assert(hashtable_capacity(ht) >= 8, "Capacity defaults to minimum");

    // Test load factor at boundary
    hashtable_destroy(&ht);
    ht = hashtable_oa_create(4, 0.5, keyops, PROBE_LINEAR, NULL);

    // Fill to just before load factor threshold
    for (int i = 0; i < 2; i++)
    {
        int k = i, v = i * 10;
        hashtable_insert(ht, &k, sizeof(int), &v);
    }

    double lf = hashtable_load_factor(ht);
    printf("Load factor before threshold: %.2f\n", lf);
    test_assert(lf <= 0.5, "Load factor at or below threshold");

    hashtable_destroy(&ht);
}

void test_oa_load_factor_stress()
{
    print_separator("Open Addressing: Load Factor Stress Test");

    HashKeyOps keyops = {
        .hash = hash_fnv1a,
        .eq = compare_int,
        .destroy_key = NULL,
        .destroy_val = NULL};

    // Test with high load factor
    HashTable *ht = hashtable_oa_create(8, 0.9, keyops, PROBE_LINEAR, NULL);
    test_assert(ht != NULL, "Create hashtable with high load factor");

    printf("Testing with load factor threshold: 0.9\n");

    int count = 0;
    for (int i = 0; i < 20; i++)
    {
        int key = i, val = i * 10;
        if (hashtable_insert(ht, &key, sizeof(int), &val))
        {
            count++;
            if (i % 5 == 0)
            {
                printf("Inserted %d keys, capacity: %zu, load: %.2f\n",
                       count, hashtable_capacity(ht), hashtable_load_factor(ht));
            }
        }
    }

    test_assert(count == 20, "All keys inserted with high load factor");
    test_assert(hashtable_size(ht) == 20, "Size correct");

    // Verify all keys accessible
    int found_count = 0;
    for (int i = 0; i < 20; i++)
    {
        if (hashtable_search(ht, &i, sizeof(int)) != NULL)
        {
            found_count++;
        }
    }
    test_assert(found_count == 20, "All keys accessible with high load");

    hashtable_destroy(&ht);
}

int main()
{
    printf("Open Addressing Hashtable Implementation Tests\n");
    printf("=============================================\n");

    test_oa_basic_operations();
    test_oa_probe_strategies();
    test_oa_collision_handling();
    test_oa_rehashing();
    test_oa_tombstone_handling();
    test_oa_string_keys();
    test_oa_edge_cases();
    test_oa_load_factor_stress();

    printf("\n✓ All open addressing hashtable tests passed!\n");
    printf("Open addressing implementation appears to be working correctly.\n");

    return 0;
}