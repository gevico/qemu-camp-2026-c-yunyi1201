#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 20 位图实现 Bloom 过滤器
 */

typedef struct {
    unsigned char *bits;
    size_t m;
} Bloom;

static Bloom *bloom_create(size_t m) {
    Bloom *bf = (Bloom*)malloc(sizeof(Bloom));
    if (!bf) return NULL;
    size_t bytes = (m + 7) / 8;
    bf->bits = (unsigned char*)calloc(bytes, 1);
    if (!bf->bits) { free(bf); return NULL; }
    bf->m = m;
    return bf;
}

static void bloom_free(Bloom *bf) {
    if (bf) {
        free(bf->bits);
        free(bf);
    }
}

static void set_bit(unsigned char *bm, size_t idx) {
    bm[idx / 8] |= (unsigned char)(1u << (idx % 8));
}

static int test_bit(const unsigned char *bm, size_t idx) {
    return (bm[idx / 8] >> (idx % 8)) & 1;
}

/* 三个简单哈希：sum(c*k) % m */
static size_t hash_k(const char *s, size_t m, int k) {
    unsigned long hash = 0;
    for (const char *p = s; *p; p++) {
        hash = hash * (unsigned long)(k + 31) + (unsigned char)*p;
    }
    return hash % m;
}

static void bloom_add(Bloom *bf, const char *s) {
    for (int k = 1; k <= 3; k++) {
        size_t idx = hash_k(s, bf->m, k);
        set_bit(bf->bits, idx);
    }
}

static int bloom_maybe_contains(Bloom *bf, const char *s) {
    for (int k = 1; k <= 3; k++) {
        size_t idx = hash_k(s, bf->m, k);
        if (!test_bit(bf->bits, idx)) return 0;
    }
    return 1;
}

int main(void) {
    const size_t m = 100;
    Bloom *bf = bloom_create(m);
    if (!bf) {
        fprintf(stderr, "bloom create failed\n");
        return 1;
    }

    bloom_add(bf, "apple");
    bloom_add(bf, "banana");

    int apple = bloom_maybe_contains(bf, "apple");
    int grape = bloom_maybe_contains(bf, "grape");

    printf("apple exists: %d\n", apple);
    printf("grape exists: %d\n", grape);

    bloom_free(bf);
    return 0;
}
