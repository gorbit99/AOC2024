#include "../utils/read_line.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define VECTOR_TYPE     char *
#define VECTOR_NAME     StringVector
#define VECTOR_FUN_NAME string
#include "../utils/vector.h"

typedef struct {
    char *pattern;
    uint64_t breakdowns;
} Breakdown;

#define VECTOR_TYPE     Breakdown
#define VECTOR_NAME     BreakdownVector
#define VECTOR_FUN_NAME breakdown
#include "../utils/vector.h"

uint64_t countBreakdowns(char *goal,
                         StringVector *patterns,
                         BreakdownVector *cache) {
    for (size_t j = 0; j < vector_breakdown_size(cache); j++) {
        Breakdown entry = vector_breakdown_at(cache, j);
        if (strcmp(goal, entry.pattern) != 0) {
            continue;
        }

        return entry.breakdowns;
    }

    size_t goalLength = strlen(goal);
    uint64_t possibleBreakdowns = 0;

    for (size_t i = 0; i < vector_string_size(patterns); i++) {
        char *pattern = vector_string_at(patterns, i);

        size_t patternLen = strlen(pattern);
        if (strncmp(pattern, goal, patternLen) != 0) {
            continue;
        }

        if (patternLen == goalLength) {
            possibleBreakdowns++;
            continue;
        }

        possibleBreakdowns +=
                countBreakdowns(goal + patternLen, patterns, cache);
    }

    vector_breakdown_push(
            cache,
            (Breakdown){.breakdowns = possibleBreakdowns, .pattern = goal});

    return possibleBreakdowns;
}

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "r");

    StringVector available = vector_string_new();
    char *patterns = readLineFromFile(input, '\n');
    size_t currentChar = 0;
    size_t length = strlen(patterns);
    while (currentChar < length) {
        int length;
        sscanf(&patterns[currentChar], "%*[^,]%n", &length);
        patterns[currentChar + length] = '\0';

        vector_string_push(&available, &patterns[currentChar]);

        currentChar += length + 2;
    }

    readLineFromFile(input, '\n');

    BreakdownVector foundBreakdowns = vector_breakdown_new();

    char *goal;
    uint64_t possibleCount = 0;
    while ((goal = readLineFromFile(input, '\n')) != NULL) {
        possibleCount += countBreakdowns(goal, &available, &foundBreakdowns);

        vector_breakdown_clear(&foundBreakdowns);

        free(goal);
    }

    printf("%lu\n", possibleCount);

    vector_breakdown_delete(&foundBreakdowns);

    free(patterns);
    vector_string_delete(&available);

    fclose(input);
    return 0;
}
