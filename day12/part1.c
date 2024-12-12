#include "../utils/read_line.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define VECTOR_TYPE     char *
#define VECTOR_NAME     StringVector
#define VECTOR_FUN_NAME string
#include "../utils/vector.h"

typedef struct {
    int x;
    int y;
} Position;

#define VECTOR_TYPE     Position
#define VECTOR_NAME     PositionVector
#define VECTOR_FUN_NAME position
#include "../utils/vector.h"

char getPosition(StringVector *map, Position pos) {
    return vector_string_at(map, pos.y)[pos.x];
}

bool isInBounds(Position pos, size_t width, size_t height) {
    return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < width;
}

bool isSame(StringVector *map,
            char c,
            Position pos,
            size_t width,
            size_t height) {
    return isInBounds(pos, width, height) && getPosition(map, pos) == c;
}

void deleteString(char **string, size_t _1, void *_2) {
    free(*string);
}

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "r");

    StringVector map = vector_string_new();

    char *line;
    while ((line = readLineFromFile(input, '\n')) != NULL) {
        vector_string_push(&map, line);
    }

    size_t width = strlen(vector_string_at(&map, 0));
    size_t height = vector_string_size(&map);

    bool **visited = malloc(sizeof(bool *) * height);
    for (size_t y = 0; y < height; y++) {
        visited[y] = calloc(width, sizeof(bool));
    }

    uint64_t total = 0;

    PositionVector visitStack = vector_position_new();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (visited[y][x]) {
                continue;
            }

            Position position = {
                    .x = x,
                    .y = y,
            };
            char currentChar = getPosition(&map, position);
            vector_position_push(&visitStack, position);

            uint64_t area = 0;
            uint64_t perimeter = 0;

            while (vector_position_size(&visitStack) != 0) {
                Position current = vector_position_pop(&visitStack);

                if (visited[current.y][current.x]) {
                    continue;
                }

                visited[current.y][current.x] = true;
                area++;

                Position left = {
                        .x = current.x - 1,
                        .y = current.y,
                };
                Position right = {
                        .x = current.x + 1,
                        .y = current.y,
                };
                Position up = {
                        .x = current.x,
                        .y = current.y - 1,
                };
                Position down = {
                        .x = current.x,
                        .y = current.y + 1,
                };

                bool leftSame = isSame(&map, currentChar, left, width, height);
                bool rightSame =
                        isSame(&map, currentChar, right, width, height);
                bool upSame = isSame(&map, currentChar, up, width, height);
                bool downSame = isSame(&map, currentChar, down, width, height);

                if (leftSame) {
                    vector_position_push(&visitStack, left);
                } else {
                    perimeter++;
                }

                if (rightSame) {
                    vector_position_push(&visitStack, right);
                } else {
                    perimeter++;
                }

                if (upSame) {
                    vector_position_push(&visitStack, up);
                } else {
                    perimeter++;
                }

                if (downSame) {
                    vector_position_push(&visitStack, down);
                } else {
                    perimeter++;
                }
            }
            total += area * perimeter;
        }
    }

    printf("%lu\n", total);

    for (size_t y = 0; y < height; y++) {
        free(visited[y]);
    }
    free(visited);

    vector_position_delete(&visitStack);
    vector_string_each(&map, deleteString, NULL);
    vector_string_delete(&map);
    fclose(input);
    return 0;
}
