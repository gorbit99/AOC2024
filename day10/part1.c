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

bool positionInBounds(Position position, size_t width, size_t height) {
    return position.x >= 0 && position.x < width && position.y >= 0
           && position.y < height;
}

char getPosition(StringVector *map, Position position) {
    return vector_string_at(map, position.y)[position.x];
}

uint64_t exploreTrails(StringVector *map,
                       Position start,
                       size_t width,
                       size_t height) {
    PositionVector stack = vector_position_new();
    vector_position_push(&stack, start);
    uint64_t trailCount = 0;

    bool **seen = malloc(sizeof(bool *) * height);
    for (size_t y = 0; y < height; y++) {
        seen[y] = calloc(width, sizeof(bool));
    }

    while (vector_position_size(&stack) > 0) {
        Position current = vector_position_pop(&stack);
        if (seen[current.y][current.x]) {
            continue;
        }

        seen[current.y][current.x] = true;

        char currentValue = getPosition(map, current);
        if (currentValue == '9') {
            trailCount++;
            continue;
        }
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

        if (positionInBounds(left, width, height)
            && getPosition(map, left) == currentValue + 1) {
            vector_position_push(&stack, left);
        }
        if (positionInBounds(right, width, height)
            && getPosition(map, right) == currentValue + 1) {
            vector_position_push(&stack, right);
        }
        if (positionInBounds(up, width, height)
            && getPosition(map, up) == currentValue + 1) {
            vector_position_push(&stack, up);
        }
        if (positionInBounds(down, width, height)
            && getPosition(map, down) == currentValue + 1) {
            vector_position_push(&stack, down);
        }
    }

    return trailCount;
}

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "r");

    StringVector map = vector_string_new();
    char *line;
    while ((line = readLineFromFile(input, '\n'))) {
        vector_string_push(&map, line);
    }

    size_t width = strlen(vector_string_at(&map, 0));
    size_t height = vector_string_size(&map);

    uint64_t total = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Position pos = {.x = x, .y = y};
            if (getPosition(&map, pos) == '0') {
                uint64_t cur = exploreTrails(&map, pos, width, height);
                total += cur;
            }
        }
    }

    printf("%lu\n", total);

    fclose(input);
}
