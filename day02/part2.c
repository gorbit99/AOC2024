#include "../utils/read_line.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define VECTOR_NAME Uint32Vector
#define VECTOR_TYPE uint32_t
#include "../utils/vector.h"

#define VECTOR_NAME IntVector
#define VECTOR_TYPE int
#include "../utils/vector.h"

bool isAcceptable(int diff, bool positive) {
    return abs(diff) >= 1 && abs(diff) <= 3 && (diff > 0) == positive;
}

bool isLineSafe(char *line) {
    Uint32Vector numbers = vector_uint32_t_new();

    while (*line != '\0') {
        uint32_t number;
        int readBytes;
        sscanf(line, "%d%n", &number, &readBytes);

        vector_uint32_t_push(&numbers, number);

        line += readBytes;
    }

    IntVector diffs = vector_int_new();
    for (size_t i = 1; i < vector_uint32_t_size(&numbers); i++) {
        uint32_t prev = vector_uint32_t_at(&numbers, i - 1);
        uint32_t cur = vector_uint32_t_at(&numbers, i);

        vector_int_push(&diffs, (int)cur - (int)prev);
    }

    vector_uint32_t_delete(&numbers);

    size_t diffCount = vector_int_size(&diffs);

    uint32_t positiveCount = 0;
    for (size_t i = 0; i < diffCount; i++) {
        if (vector_int_at(&diffs, i) > 0) {
            positiveCount++;
        }
    }

    bool positive = positiveCount > diffCount - positiveCount;

    bool dampenerApplied = false;
    bool safe = true;
    for (size_t i = 0; i < diffCount; i++) {
        int curDiff = vector_int_at(&diffs, i);
        if (isAcceptable(curDiff, positive)) {
            continue;
        }

        if (dampenerApplied) {
            safe = false;
            break;
        }

        if (i < diffCount - 1) {
            int sumDiff = curDiff + vector_int_at(&diffs, i + 1);
            if (isAcceptable(sumDiff, positive)) {
                dampenerApplied = true;
                i++;
                continue;
            }
        } else {
            dampenerApplied = true;
            continue;
        }

        if (i > 0) {
            int sumDiff = curDiff + vector_int_at(&diffs, i - 1);
            if (isAcceptable(sumDiff, positive)) {
                dampenerApplied = true;
                continue;
            }
        } else {
            dampenerApplied = true;
            continue;
        }

        safe = false;
        break;
    }

    vector_int_delete(&diffs);
    return safe;
}

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "r");

    uint32_t safeCount = 0;

    char *line = readLineFromFile(input, '\n');
    while (line != NULL) {
        if (isLineSafe(line)) {
            safeCount++;
        }

        free(line);
        line = readLineFromFile(input, '\n');
    }

    printf("%u\n", safeCount);

    return 0;
}
