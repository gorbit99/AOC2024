#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#if defined VECTOR_NAME && defined VECTOR_TYPE
#define PASTER(x, y) x##y
#define GLUE(x, y)   PASTER(x, y)

#define VECTOR_FUN(name) GLUE(GLUE(vector_, VECTOR_TYPE), GLUE(_, name))

typedef struct {
    VECTOR_TYPE *data;
    size_t size;
    size_t capacity;
} VECTOR_NAME;

VECTOR_NAME VECTOR_FUN(new)() {
    return (VECTOR_NAME){
            .size = 0,
            .capacity = 0,
            .data = NULL,
    };
}

void VECTOR_FUN(delete)(VECTOR_NAME *vec) {
    free(vec->data);
    vec->data = NULL;
}

size_t VECTOR_FUN(size)(VECTOR_NAME *vec) {
    return vec->size;
}

VECTOR_TYPE VECTOR_FUN(at)(VECTOR_NAME *vec, size_t i) {
    assert(i < vec->size);

    return vec->data[i];
}

void VECTOR_FUN(reserve)(VECTOR_NAME *vec, size_t newCapacity) {
    if (vec->capacity >= newCapacity) {
        return;
    }

    vec->data = realloc(vec->data, sizeof(VECTOR_TYPE) * newCapacity);
    vec->capacity = newCapacity;
}

void VECTOR_FUN(push)(VECTOR_NAME *vec, VECTOR_TYPE value) {
    if (vec->size == vec->capacity) {
        VECTOR_FUN(reserve)(vec, vec->capacity == 0 ? 8 : vec->capacity * 2);
    }

    vec->data[vec->size] = value;
    vec->size++;
}

VECTOR_TYPE *VECTOR_FUN(top)(VECTOR_NAME *vec) {
    assert(vec->size > 0);

    return &vec->data[vec->size - 1];
}

VECTOR_TYPE VECTOR_FUN(pop)(VECTOR_NAME *vec) {
    assert(vec->size > 0);

    VECTOR_TYPE value = vec->data[vec->size - 1];
    vec->size--;
    return value;
}

void VECTOR_FUN(unshift)(VECTOR_NAME *vec, VECTOR_TYPE value) {
    if (vec->size == vec->capacity) {
        VECTOR_FUN(reserve)(vec, vec->capacity == 0 ? 8 : vec->capacity * 2);
    }

    memmove(vec->data + 1, vec->data, sizeof(VECTOR_TYPE) * vec->size);
    vec->data[0] = value;
}

VECTOR_TYPE VECTOR_FUN(shift)(VECTOR_NAME *vec) {
    assert(vec->size > 0);

    VECTOR_TYPE value = vec->data[0];
    vec->size--;
    memmove(vec->data, vec->data + 1, sizeof(VECTOR_TYPE) * vec->size);
    return value;
}

void VECTOR_FUN(each)(VECTOR_NAME *vec,
                      void (*callback)(VECTOR_TYPE *value,
                                       size_t i,
                                       void *userdata),
                      void *userdata) {
    for (size_t i = 0; i < vec->size; i++) {
        callback(&vec->data[i], i, userdata);
    }
}

void VECTOR_FUN(map)(VECTOR_NAME *vec,
                     VECTOR_TYPE (*callback)(VECTOR_TYPE *value,
                                             size_t i,
                                             void *userdata),
                     void *userdata) {
    for (size_t i = 0; i < vec->size; i++) {
        vec->data[i] = callback(&vec->data[i], i, userdata);
    }
}

void *VECTOR_FUN(fold)(VECTOR_NAME *vec,
                       void (*callback)(VECTOR_TYPE *value,
                                        size_t i,
                                        void *startValue,
                                        void *userdata),
                       void *startValue,
                       void *userdata) {
    for (size_t i = 0; i < vec->size; i++) {
        callback(&vec->data[i], i, startValue, userdata);
    }
    return startValue;
}

VECTOR_TYPE *VECTOR_FUN(data)(VECTOR_NAME *vec) {
    return vec->data;
}

void VECTOR_FUN(clear)(VECTOR_NAME *vec) {
    vec->size = 0;
}

#endif

#undef VECTOR_NAME
#undef VECTOR_TYPE
