#include "../utils/read_line.h"

#include <stdbool.h>
#include <stdio.h>

#define VECTOR_TYPE     char *
#define VECTOR_NAME     StringVector
#define VECTOR_FUN_NAME string
#include "../utils/vector.h"

void deleteString(char **string, size_t _1, void *_2) {
    free(*string);
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

    StringVector checkStack = vector_string_new();
    StringVector alreadyChecked = vector_string_new();

    readLineFromFile(input, '\n');

    char *goal;
    uint64_t possibleCount = 0;
    while ((goal = readLineFromFile(input, '\n')) != NULL) {
        vector_string_clear(&checkStack);
        vector_string_clear(&alreadyChecked);
        vector_string_push(&checkStack, goal);

        bool found = false;
        while (vector_string_size(&checkStack) > 0) {
            char *current = vector_string_pop(&checkStack);
            size_t currentLen = strlen(current);

            for (size_t i = 0; i < vector_string_size(&available); i++) {
                char *pattern = vector_string_at(&available, i);

                size_t patternLen = strlen(pattern);
                if (strncmp(pattern, current, patternLen) != 0) {
                    continue;
                }

                if (patternLen == currentLen) {
                    possibleCount++;
                    found = true;
                    break;
                }

                bool isAlreadyChecked = false;
                for (size_t j = 0; j < vector_string_size(&alreadyChecked);
                     j++) {
                    char *entry = vector_string_at(&alreadyChecked, j);
                    if (strcmp(current + patternLen, entry) != 0) {
                        continue;
                    }

                    isAlreadyChecked = true;
                    break;
                }

                if (!isAlreadyChecked) {
                    vector_string_push(&checkStack, current + patternLen);
                }
            }

            if (found) {
                break;
            }

            vector_string_push(&alreadyChecked, current);
        }

        free(goal);
    }

    printf("%lu\n", possibleCount);

    vector_string_delete(&checkStack);
    vector_string_delete(&alreadyChecked);

    free(patterns);
    vector_string_delete(&available);

    fclose(input);
    return 0;
}
