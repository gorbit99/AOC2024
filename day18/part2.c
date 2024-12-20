#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAP_SIZE 71

typedef struct {
    int x;
    int y;
    uint64_t distance;
} Position;

#define VECTOR_NAME     PositionVector
#define VECTOR_TYPE     Position
#define VECTOR_FUN_NAME position
#include "../utils/vector.h"

bool positionIsInBounds(Position pos) {
    return pos.x >= 0 && pos.x < MAP_SIZE && pos.y >= 0 && pos.y < MAP_SIZE;
}

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "r");

    char **map = malloc(sizeof(char *) * MAP_SIZE);
    for (size_t y = 0; y < MAP_SIZE; y++) {
        map[y] = malloc(sizeof(char) * (MAP_SIZE + 1));
        memset(map[y], '.', sizeof(char) * MAP_SIZE);
        map[y][MAP_SIZE] = '\0';
    }

    bool **seen = malloc(sizeof(bool *) * MAP_SIZE);
    for (size_t y = 0; y < MAP_SIZE; y++) {
        seen[y] = calloc(MAP_SIZE, sizeof(bool));
    }

    PositionVector queue = vector_position_new();
    while (true) {
        int x;
        int y;
        fscanf(input, "%d,%d\n", &x, &y);
        map[y][x] = '#';

        for (size_t y = 0; y < MAP_SIZE; y++) {
            memset(seen[y], 0, sizeof(bool) * MAP_SIZE);
        }

        vector_position_clear(&queue);
        vector_position_push(&queue, (Position){.x = 0, .y = 0, .distance = 0});
        bool found = false;
        while (vector_position_size(&queue) > 0) {
            Position current = vector_position_shift(&queue);

            if (!positionIsInBounds(current)) {
                continue;
            }

            if (map[current.y][current.x] == '#') {
                continue;
            }

            if (seen[current.y][current.x]) {
                continue;
            }
            seen[current.y][current.x] = true;

            if (current.x == MAP_SIZE - 1 && current.y == MAP_SIZE - 1) {
                found = true;
                break;
            }

            vector_position_push(&queue,
                                 (Position){.x = current.x - 1,
                                            .y = current.y,
                                            .distance = current.distance + 1});
            vector_position_push(&queue,
                                 (Position){.x = current.x + 1,
                                            .y = current.y,
                                            .distance = current.distance + 1});
            vector_position_push(&queue,
                                 (Position){.x = current.x,
                                            .y = current.y - 1,
                                            .distance = current.distance + 1});
            vector_position_push(&queue,
                                 (Position){.x = current.x,
                                            .y = current.y + 1,
                                            .distance = current.distance + 1});
        }

        if (!found) {
            printf("%d,%d\n", x, y);
            break;
        }
    }

    fclose(input);
    return 0;
}
