#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 17 位图操作
 */

static void set_bit(unsigned char* bitmap, size_t bit_index) {
    size_t byte = bit_index / 8;
    size_t offset = bit_index % 8;
    bitmap[byte] |= (unsigned char)(1u << offset);
}

static int test_bit(const unsigned char* bitmap, size_t bit_index) {
    size_t byte = bit_index / 8;
    size_t offset = bit_index % 8;
    return (bitmap[byte] >> offset) & 1;
}

int main(void) {
    const size_t bits = 10;
    const size_t bytes = (bits + 7) / 8;
    unsigned char* bitmap = (unsigned char*)calloc(bytes, sizeof(unsigned char));
    if (!bitmap) {
        fprintf(stderr, "内存分配失败\n");
        return 1;
    }

    set_bit(bitmap, 0);
    set_bit(bitmap, 3);
    set_bit(bitmap, 5);

    int b3 = test_bit(bitmap, 3);
    int b1 = test_bit(bitmap, 1);
    printf("query(3)=%d\n", b3);
    printf("query(1)=%d\n", b1);

    free(bitmap);
    return 0;
}
