#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "bitwise_utils.h"

void test_basic_operations(void)
{
    uint32_t x = 0b10110100; // 180

    printf("第2位是否为1: %s\n", bit_is_set(x, 2) ? "是" : "否"); // 是
    printf("设置第0位: %u\n", bit_set(x, 0));                     // 181
    printf("清除第7位: %u\n", bit_clear(x, 7));                   // 52
    printf("翻转第1位: %u\n", bit_flip(x, 1));                    // 182
    printf("最低4位: %u\n", get_low_bits(x, 4));                  // 4
}

void test_counting_functions(void)
{
    uint32_t x = 0b10110100; // 180

    printf("1的个数: %d\n", count_set_bits(x));                   // 4
    printf("0的个数: %d\n", count_clear_bits(x));                 // 28（32位）
    printf("最右边1的位置: %d\n", find_rightmost_set_bit(x));     // 2
    printf("最左边1的位置: %d\n", find_leftmost_set_bit(x));      // 7
    printf("是否为2的幂: %s\n", is_power_of_2(16) ? "是" : "否"); // 是
}

void test_pattern_operations(void)
{
    uint32_t x = 0b10110100; // 180

    printf("反转位: ");
    print_binary(reverse_bits(x), 32);
    printf("左移31位: ");
    print_binary(rotate_left(x, 31), 32);
    printf("右移3位: ");
    print_binary(rotate_right(x, 3), 32);
    printf("交换第1位和第5位: ");
    print_binary(swap_bits(x, 1, 5), 32);
    printf("提取第2-5位: ");
    print_binary(extract_bits(x, 2, 4), 32);
}

void test_utility_functions(void)
{
    uint32_t x = 0b10110100; // 180
    uint32_t y = 0b00000011; // 3

    printf("hamming distance: %d\n", hamming_distance(x, y)); // 6
    printf("7 is even? %d\n", is_even(7));
    printf("swap x y:\n");
    swap_without_temp(&x, &y);
    printf("x, y = %d, %d\n", x, y); // 3, 180
    printf("3 * 2^2 = %d\n", multiply_by_power_of_2(x, 2));
}

void test_flag_manager(void)
{
    printf("Running flag manager tests...\n");

    FlagManager *fm = flag_manager_create();

    // 初始状态
    printf("Initial flags: %s\n", flag_to_string(fm));

    // 设置 ACTIVE 和 DIRTY
    flag_set(fm, FLAG_ACTIVE);
    flag_set(fm, FLAG_DIRTY);
    printf("Set ACTIVE + DIRTY: %s\n", flag_to_string(fm));

    // 检查 ACTIVE 是否设置
    printf("ACTIVE is %s\n", flag_is_set(fm, FLAG_ACTIVE) ? "SET" : "NOT SET");

    // 清除 ACTIVE
    flag_clear(fm, FLAG_ACTIVE);
    printf("Cleared ACTIVE: %s\n", flag_to_string(fm));

    // 切换 VISIBLE
    flag_toggle(fm, FLAG_VISIBLE);
    printf("Toggled VISIBLE: %s\n", flag_to_string(fm));

    // 批量设置 LOCKED + SELECTED
    flag_set_multiple(fm, FLAG_LOCKED | FLAG_SELECTED);
    printf("Set LOCKED + SELECTED: %s\n", flag_to_string(fm));

    // 清除所有
    flag_clear_all(fm);
    printf("Cleared ALL flags: %s\n", flag_to_string(fm));

    flag_manager_destroy(&fm);
    printf("Flag manager tests completed.\n");
}

void test_bitarray(void) {
    printf("Running BitArray tests...\n");

    size_t count = 1000;
    int bits_per_item = 5;  // 支持0~31
    BitArray *ba = bitarray_create(count, bits_per_item);
    assert(ba != NULL);

    // 初始化所有元素为 index % 32
    for (size_t i = 0; i < count; i++) {
        uint32_t value = i % 32;
        bitarray_set(ba, i, value);
    }

    // 检查读取是否一致
    for (size_t i = 0; i < count; i++) {
        uint32_t expected = i % 32;
        uint32_t actual = bitarray_get(ba, i);
        assert(expected == actual);
    }

    printf("✅ Basic set/get test passed.\n");

    // 随机测试
    srand((unsigned)time(NULL));
    for (size_t i = 0; i < count; i++) {
        uint32_t value = rand() % 32;
        bitarray_set(ba, i, value);
    }

    for (size_t i = 0; i < count; i++) {
        uint32_t expected = bitarray_get(ba, i);  // 读取应该等于刚写入
        uint32_t actual = bitarray_get(ba, i);
        assert(expected == actual);
    }

    printf("✅ Random test passed.\n");

    bitarray_destroy(&ba);
    assert(ba == NULL);
    printf("✅ Destroy test passed.\n");

    printf("All BitArray tests passed!\n");
}

void test_xor_encrypt_decrypt(void)
{
    char message[] = "Hello, Bitwise World!";
    char key[] = "mykey";
    size_t msg_len = strlen(message);

    xor_encrypt_decrypt((uint8_t*)message, msg_len, key);
    // 打印加密后的十六进制（防止被 \0 截断）
    printf("加密后: ");
    for (size_t i = 0; i < msg_len; i++) {
        printf("%02X ", (unsigned char)message[i]);
    }
    printf("\n");
    
    xor_encrypt_decrypt((uint8_t*)message, msg_len, key);
    printf("解密后: %s\n", message);  // 应该恢复原文
}

int main()
{
    test_xor_encrypt_decrypt();
    return 0;
}