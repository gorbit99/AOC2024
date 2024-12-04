#include "../utils/read_line.h"

#include <stdbool.h>
#include <stdio.h>

#define VECTOR_TYPE     char *
#define VECTOR_NAME     StringVector
#define VECTOR_FUN_NAME string
#include "../utils/vector.h"

bool isOutOfBounds(int x, int y, size_t width, size_t height) {
    return x < 0 || x >= width || y < 0 || y >= height;
}

char getChar(StringVector *lines, int x, int y) {
    return vector_string_at(lines, y)[x];
}

size_t doSearch(StringVector *lines,
                int x,
                int y,
                size_t width,
                size_t height,
                char *searchText,
                size_t searchLen) {
    if (getChar(lines, x, y) != searchText[0]) {
        return 0;
    }

    int dxMap[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int dyMap[8] = {0, -1, -1, -1, 0, 1, 1, 1};
    size_t count = 0;

    for (size_t dir = 0; dir < 8; dir++) {
        bool good = true;
        for (size_t i = 1; i < searchLen; i++) {
            int dx = dxMap[dir] * i;
            int dy = dyMap[dir] * i;

            if (isOutOfBounds(x + dx, y + dy, width, height)
                || getChar(lines, x + dx, y + dy) != searchText[i]) {
                good = false;
                break;
            }
        }

        if (good) {
            count++;
        }
    }

    return count;
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

    char *search = "XMAS";
    size_t searchLen = strlen(search);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            xmasCount +=
                    doSearch(&lines, x, y, width, height, search, searchLen);
        }
    }

    printf("%zu\n", xmasCount);

    vector_string_each(&lines, deleteString, NULL);
    vector_string_delete(&lines);

    fclose(input);
    return 0;
}
