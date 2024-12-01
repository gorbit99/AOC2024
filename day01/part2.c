#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int numberComparator(const void *a, const void *b) {
    return *(uint32_t *)a - *(uint32_t *)b;
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

    uint32_t similarity = 0;

    size_t index1 = 0;
    size_t index2 = 0;
    size_t currentCount = 0;

    while (index1 < line_count && index2 < line_count) {
        if (array1[index1] == array2[index2]) {
            currentCount++;
            index2++;
            continue;
        }

        if (array1[index1] < array2[index2]) {
            similarity += currentCount * array1[index1];
            while (index1 + 1 < line_count
                   && array1[index1] == array1[index1 + 1]) {
                similarity += currentCount * array1[index1];
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
