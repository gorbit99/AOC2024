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

void differenceCalculator(uint32_t *a, size_t i, void *sum, void *vectorB) {
    uint32_t b = vector_uint32_t_at(vectorB, i);

    uint32_t *totalDifference = sum;
    *totalDifference += abs((int)*a - (int)b);
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

    qsort(vector_uint32_t_data(&vectorA),
          vector_uint32_t_size(&vectorA),
          sizeof(uint32_t),
          numberComparator);
    qsort(vector_uint32_t_data(&vectorB),
          vector_uint32_t_size(&vectorB),
          sizeof(uint32_t),
          numberComparator);

    uint32_t totalDifference = 0;
    vector_uint32_t_fold(&vectorA,
                         differenceCalculator,
                         &totalDifference,
                         &vectorB);

    printf("%u\n", totalDifference);

    return 0;
}
