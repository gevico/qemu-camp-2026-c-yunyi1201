#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GARRAY_INIT_CAP 16U

typedef struct {
    void* data;
    size_t len;
    size_t capacity;
    size_t elem_size;
} GArray;

GArray* garray_init(size_t elem_size) {
    GArray *arr = (GArray*)malloc(sizeof(GArray));
    if (!arr) return NULL;
    arr->data = malloc(GARRAY_INIT_CAP * elem_size);
    if (!arr->data) { free(arr); return NULL; }
    arr->len = 0;
    arr->capacity = GARRAY_INIT_CAP;
    arr->elem_size = elem_size;
    return arr;
}

void garray_append(GArray* arr, void* elem) {
    if (arr->len >= arr->capacity) {
        size_t new_cap = arr->capacity * 2;
        void *new_data = realloc(arr->data, new_cap * arr->elem_size);
        if (!new_data) return;
        arr->data = new_data;
        arr->capacity = new_cap;
    }
    memcpy((char*)arr->data + arr->len * arr->elem_size, elem, arr->elem_size);
    arr->len++;
}

void garray_free(GArray* arr) {
    if (arr) {
        free(arr->data);
        free(arr);
    }
}

int main(void) {
    GArray* a = garray_init(sizeof(int));
    if (!a) {
        fprintf(stderr, "初始化失败\n");
        return 1;
    }

    for (int i = 1; i <= 17; ++i) {
        garray_append(a, &i);
    }
    printf("len=%zu\n", a->len);
    printf("capacity=%zu\n", a->capacity);
    int* arr_i = (int*)a->data;
    printf("arr[0]=%d\n", arr_i[0]);
    printf("arr[16]=%d\n", arr_i[16]);

    int ok = (a->len == 17 && a->capacity == 32 && arr_i[0] == 1 && arr_i[16] == 17);
    if (!ok) {
        garray_free(a);
        return 1;
    }

    garray_free(a);
    return 0;
}
