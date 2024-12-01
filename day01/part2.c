#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define VECTOR_NAME Uint32Vector
#define VECTOR_TYPE uint32_t
#include "../utils/vector.h"

int numberComparator(const void *a, const void *b) {
    return *(uint32_t *)a - *(uint32_t *)b;
}

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "r");

    Uint32Vector vectorA = vector_uint32_t_new();
    Uint32Vector vectorB = vector_uint32_t_new();

    uint32_t a;
    uint32_t b;
    while (fscanf(input, "%d %d\n", &a, &b) != EOF) {
        vector_uint32_t_push(&vectorA, a);
        vector_uint32_t_push(&vectorB, b);
    }

    size_t line_count = vector_uint32_t_size(&vectorA);

    qsort(vector_uint32_t_data(&vectorA),
          line_count,
          sizeof(uint32_t),
          numberComparator);
    qsort(vector_uint32_t_data(&vectorB),
          line_count,
          sizeof(uint32_t),
          numberComparator);

    uint32_t similarity = 0;

    size_t index1 = 0;
    size_t index2 = 0;
    size_t currentCount = 0;

    while (index1 < line_count && index2 < line_count) {
        uint32_t a = vector_uint32_t_at(&vectorA, index1);
        uint32_t b = vector_uint32_t_at(&vectorB, index2);

        if (a == b) {
            currentCount++;
            index2++;
            continue;
        }

        if (a < b) {
            similarity += currentCount * a;
            while (index1 + 1 < line_count
                   && a == vector_uint32_t_at(&vectorA, index1 + 1)) {
                similarity += currentCount * a;
                index1++;
            }
            index1++;
            currentCount = 0;
            continue;
        }

        index2++;
    }

    printf("%u\n", similarity);

    return 0;
}
