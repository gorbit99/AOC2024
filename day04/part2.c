#include "../utils/read_line.h"

#include <stdbool.h>
#include <stdio.h>

#define VECTOR_TYPE     char *
#define VECTOR_NAME     StringVector
#define VECTOR_FUN_NAME string
#include "../utils/vector.h"

char getChar(StringVector *lines, int x, int y) {
    return vector_string_at(lines, y)[x];
}

void deleteString(char **string, size_t _1, void *_2) {
    free(*string);
}

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "r");

    StringVector lines = vector_string_new();

    char *line = readLineFromFile(input, '\n');
    while (line != NULL) {
        vector_string_push(&lines, line);
        line = readLineFromFile(input, '\n');
    }

    size_t xmasCount = 0;
    size_t width = strlen(vector_string_at(&lines, 0));
    size_t height = vector_string_size(&lines);

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            if (getChar(&lines, x, y) != 'A') {
                continue;
            }

            bool axisOne = (getChar(&lines, x - 1, y - 1) == 'M'
                            && getChar(&lines, x + 1, y + 1) == 'S')
                           || (getChar(&lines, x - 1, y - 1) == 'S'
                               && getChar(&lines, x + 1, y + 1) == 'M');
            bool axisTwo = (getChar(&lines, x + 1, y - 1) == 'M'
                            && getChar(&lines, x - 1, y + 1) == 'S')
                           || (getChar(&lines, x + 1, y - 1) == 'S'
                               && getChar(&lines, x - 1, y + 1) == 'M');

            if (axisOne && axisTwo) {
                xmasCount++;
            }
        }
    }

    printf("%zu\n", xmasCount);

    vector_string_each(&lines, deleteString, NULL);
    vector_string_delete(&lines);

    fclose(input);
    return 0;
}
