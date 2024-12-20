#include "../utils/read_line.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define VECTOR_TYPE     char *
#define VECTOR_NAME     StringVector
#define VECTOR_FUN_NAME string
#include "../utils/vector.h"

typedef struct {
    int x;
    int y;
} Position;

bool isInBounds(Position pos, size_t width, size_t height) {
    return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < width;
}

char getChar(StringVector *map, Position position) {
    return vector_string_at(map, position.y)[position.x];
}

typedef struct {
    Position position;
    uint64_t distance;
} State;

#define VECTOR_TYPE     State
#define VECTOR_NAME     StateVector
#define VECTOR_FUN_NAME state
#include "../utils/vector.h"

typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN,
} Direction;

Position positionMove(Position position, Direction direction) {
    switch (direction) {
    case LEFT:
        position.x--;
        break;
    case RIGHT:
        position.x++;
        break;
    case UP:
        position.y--;
        break;
    case DOWN:
        position.y++;
        break;
    }
    return position;
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

    Position start;
    bool found = false;
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            if (getChar(&map, (Position){.x = x, .y = y}) == 'S') {
                start.x = x;
                start.y = y;
                found = true;
                break;
            }
        }

        if (found) {
            break;
        }
    }

    bool **seen = malloc(sizeof(bool *) * height);
    for (size_t y = 0; y < height; y++) {
        seen[y] = calloc(width, sizeof(bool));
    }

    uint64_t **distances = malloc(sizeof(uint64_t *) * height);
    for (size_t y = 0; y < height; y++) {
        distances[y] = calloc(width, sizeof(uint64_t));
    }

    StateVector queue = vector_state_new();
    vector_state_push(&queue, (State){.position = start, .distance = 0});
    while (vector_state_size(&queue) > 0) {
        State current = vector_state_shift(&queue);

        if (seen[current.position.y][current.position.x]) {
            continue;
        }
        seen[current.position.y][current.position.x] = true;
        distances[current.position.y][current.position.x] = current.distance;

        if (getChar(&map, current.position) == '#') {
            continue;
        }

        if (getChar(&map, current.position) == 'E') {
            break;
        }

        Position left = positionMove(current.position, LEFT);
        Position right = positionMove(current.position, RIGHT);
        Position up = positionMove(current.position, UP);
        Position down = positionMove(current.position, DOWN);

        vector_state_push(
                &queue,
                (State){.position = left, .distance = current.distance + 1});
        vector_state_push(
                &queue,
                (State){.position = right, .distance = current.distance + 1});
        vector_state_push(
                &queue,
                (State){.position = up, .distance = current.distance + 1});
        vector_state_push(
                &queue,
                (State){.position = down, .distance = current.distance + 1});
    }

    uint64_t goodCheats = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Position currentPos = {.x = x, .y = y};
            if (getChar(&map, currentPos) == '#') {
                continue;
            }

            uint64_t currentDistance = distances[y][x];

            for (size_t cheatLength = 2; cheatLength <= 20; cheatLength++) {
                for (uint8_t directions = 0b00; directions <= 0b11;
                     directions++) {
                    int vertical = (directions & 0b01) == 0 ? -1 : 1;
                    int horizontal = (directions & 0b10) == 0 ? -1 : 1;

                    for (size_t horizontalMovements = 0;
                         horizontalMovements <= cheatLength;
                         horizontalMovements++) {
                        size_t verticalMovements =
                                cheatLength - horizontalMovements;

                        if ((horizontalMovements == 0 && horizontal != -1)
                            || (verticalMovements == 0 && vertical != -1)) {
                            continue;
                        }

                        Position possibleCheat = {
                                .x = x + (int)horizontalMovements * horizontal,
                                .y = y + (int)verticalMovements * vertical,
                        };

                        if (!isInBounds(possibleCheat, width, height)) {
                            continue;
                        }

                        if (getChar(&map, possibleCheat) == '#') {
                            continue;
                        }

                        uint64_t cheatDistance =
                                distances[possibleCheat.y][possibleCheat.x];

                        if (cheatDistance
                            >= currentDistance + 100 + cheatLength) {
                            goodCheats++;
                        }
                    }
                }
            }
        }
    }

    printf("%lu\n", goodCheats);

    for (size_t y = 0; y < height; y++) {
        free(distances[y]);
        free(seen[y]);
    }
    free(distances);
    free(seen);

    vector_state_delete(&queue);

    vector_string_each(&map, deleteString, NULL);
    vector_string_delete(&map);

    fclose(input);
    return 0;
}
