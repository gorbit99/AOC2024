#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int numberComparator(const void *a, const void *b) {
    return *(uint32_t *)b - *(uint32_t *)a;
}

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "r");

    size_t line_count = 0;
    while (fscanf(input, "%*[^\n]%*c") != EOF) {
        line_count++;
    }

    uint32_t *array1 = malloc(sizeof(uint32_t) * line_count);
    uint32_t *array2 = malloc(sizeof(uint32_t) * line_count);

    fseek(input, 0, SEEK_SET);

    uint32_t a;
    uint32_t b;
    size_t line = 0;
    while (fscanf(input, "%d %d\n", &a, &b) != EOF) {
        array1[line] = a;
        array2[line] = b;
        line++;
    }

    qsort(array1, line_count, sizeof(uint32_t), numberComparator);
    qsort(array2, line_count, sizeof(uint32_t), numberComparator);

    uint32_t totalDifference = 0;
    for (size_t i = 0; i < line_count; i++) {
        totalDifference += abs((int)array1[i] - (int)array2[i]);
    }

    printf("%u\n", totalDifference);

    return 0;
}
