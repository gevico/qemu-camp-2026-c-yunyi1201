#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 16 LRU 缓存淘汰算法（哈希表 + 双向链表）
 */

typedef struct LRUNode {
    int key;
    int value;
    struct LRUNode* prev;
    struct LRUNode* next;
} LRUNode;

typedef struct HashEntry {
    int key;
    LRUNode* node;
    struct HashEntry* next;
} HashEntry;

typedef struct {
    int capacity;
    int size;
    LRUNode* head; /* 最近使用（MRU） */
    LRUNode* tail; /* 最久未使用（LRU） */
    size_t bucket_count;
    HashEntry** buckets;
} LRUCache;

static unsigned hash_int(int key) {
    return (unsigned)key * 2654435761u;
}

static HashEntry* hash_find(LRUCache* c, int key, HashEntry*** pprev_next) {
    unsigned idx = hash_int(key) % (unsigned)c->bucket_count;
    HashEntry **pp = &c->buckets[idx];
    while (*pp) {
        if ((*pp)->key == key) {
            if (pprev_next) *pprev_next = pp;
            return *pp;
        }
        pp = &(*pp)->next;
    }
    if (pprev_next) *pprev_next = pp;
    return NULL;
}

static void list_add_to_head(LRUCache* c, LRUNode* node) {
    node->prev = NULL;
    node->next = c->head;
    if (c->head) c->head->prev = node;
    c->head = node;
    if (c->tail == NULL) c->tail = node;
}

static void list_remove(LRUCache* c, LRUNode* node) {
    if (node->prev) node->prev->next = node->next;
    else c->head = node->next;
    if (node->next) node->next->prev = node->prev;
    else c->tail = node->prev;
    node->prev = node->next = NULL;
}

static void list_move_to_head(LRUCache* c, LRUNode* node) {
    list_remove(c, node);
    list_add_to_head(c, node);
}

static LRUNode* list_pop_tail(LRUCache* c) {
    if (!c->tail) return NULL;
    LRUNode *node = c->tail;
    list_remove(c, node);
    return node;
}

static LRUCache* lru_create(int capacity) {
    LRUCache *c = (LRUCache*)malloc(sizeof(LRUCache));
    c->capacity = capacity;
    c->size = 0;
    c->head = c->tail = NULL;
    c->bucket_count = (size_t)(capacity * 2 + 1);
    c->buckets = (HashEntry**)calloc(c->bucket_count, sizeof(HashEntry*));
    return c;
}

static void lru_free(LRUCache* c) {
    LRUNode *p = c->head;
    while (p) {
        LRUNode *tmp = p;
        p = p->next;
        free(tmp);
    }
    for (size_t i = 0; i < c->bucket_count; i++) {
        HashEntry *e = c->buckets[i];
        while (e) {
            HashEntry *tmp = e;
            e = e->next;
            free(tmp);
        }
    }
    free(c->buckets);
    free(c);
}

static int lru_get(LRUCache* c, int key, int* out_value) {
    HashEntry *e = hash_find(c, key, NULL);
    if (!e) return 0;
    list_move_to_head(c, e->node);
    if (out_value) *out_value = e->node->value;
    return 1;
}

static void lru_put(LRUCache* c, int key, int value) {
    HashEntry *e = hash_find(c, key, NULL);
    if (e) {
        e->node->value = value;
        list_move_to_head(c, e->node);
        return;
    }

    // New entry
    LRUNode *node = (LRUNode*)malloc(sizeof(LRUNode));
    node->key = key;
    node->value = value;
    node->prev = node->next = NULL;
    list_add_to_head(c, node);

    // Add to hash
    unsigned idx = hash_int(key) % (unsigned)c->bucket_count;
    HashEntry *he = (HashEntry*)malloc(sizeof(HashEntry));
    he->key = key;
    he->node = node;
    he->next = c->buckets[idx];
    c->buckets[idx] = he;

    c->size++;

    if (c->size > c->capacity) {
        // Evict LRU
        LRUNode *lru = list_pop_tail(c);
        // Remove from hash
        HashEntry **pp;
        HashEntry *old = hash_find(c, lru->key, &pp);
        if (old) {
            *pp = old->next;
            free(old);
        }
        free(lru);
        c->size--;
    }
}

static void lru_print(LRUCache* c) {
    LRUNode* p = c->head;
    int first = 1;
    while (p) {
        if (!first) printf(", ");
        first = 0;
        printf("%d:%d", p->key, p->value);
        p = p->next;
    }
    printf("\n");
}

int main(void) {
    LRUCache* c = lru_create(3);
    if (!c) {
        fprintf(stderr, "创建 LRU 失败\n");
        return 1;
    }

    lru_put(c, 1, 1);
    lru_put(c, 2, 2);
    lru_put(c, 3, 3);
    lru_put(c, 4, 4);

    int val;
    if (lru_get(c, 2, &val)) {
        (void)val;
    }

    lru_put(c, 5, 5);

    lru_print(c);

    lru_free(c);
    return 0;
}
