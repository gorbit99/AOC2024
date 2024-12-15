#include "../utils/read_line.h"

#include <stdbool.h>
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

Position positionAdd(Position a, Position b) {
    return (Position){
            .x = a.x + b.x,
            .y = a.y + b.y,
    };
}

typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN,
} Move;

Position moveToDirection(Move move) {
    switch (move) {
    case LEFT:
        return (Position){.x = -1, .y = 0};
    case RIGHT:
        return (Position){.x = 1, .y = 0};
    case UP:
        return (Position){.x = 0, .y = -1};
    case DOWN:
        return (Position){.x = 0, .y = 1};
    }
    return (Position){.x = 0, .y = 0};
}

#define VECTOR_TYPE     Move
#define VECTOR_NAME     MoveVector
#define VECTOR_FUN_NAME move
#include "../utils/vector.h"

char getChar(StringVector *map, Position position) {
    return vector_string_at(map, position.y)[position.x];
}

void setChar(StringVector *map, Position position, char c) {
    vector_string_at(map, position.y)[position.x] = c;
}

bool isInBounds(Position position, size_t width, size_t height) {
    return position.x >= 0 && position.x < width && position.y >= 0
           && position.y < height;
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

    MoveVector moves = vector_move_new();
    while ((line = readLineFromFile(input, '\n')) != NULL) {
        for (size_t i = 0; line[i] != '\0'; i++) {
            switch (line[i]) {
            case '<':
                vector_move_push(&moves, LEFT);
                break;
            case '>':
                vector_move_push(&moves, RIGHT);
                break;
            case '^':
                vector_move_push(&moves, UP);
                break;
            case 'v':
                vector_move_push(&moves, DOWN);
                break;
            }
        }
        free(line);
    }

    Position robot;
    bool found = false;
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            if (getChar(&map, (Position){.x = x, .y = y}) == '@') {
                robot.x = x;
                robot.y = y;
                found = true;
                break;
            }
        }

        if (found) {
            break;
        }
    }

    for (size_t i = 0; i < vector_move_size(&moves); i++) {
        Move move = vector_move_at(&moves, i);
        Position direction = moveToDirection(move);

        Position nextPosition = positionAdd(robot, direction);
        while (getChar(&map, nextPosition) == 'O') {
            nextPosition = positionAdd(nextPosition, direction);
        }

        if (getChar(&map, nextPosition) == '#') {
            continue;
        }

        Position reverseDir = {
                .x = -direction.x,
                .y = -direction.y,
        };

        while (robot.x != nextPosition.x || robot.y != nextPosition.y) {
            Position pushed = positionAdd(nextPosition, reverseDir);
            setChar(&map, nextPosition, getChar(&map, pushed));
            setChar(&map, pushed, '.');
            nextPosition = pushed;
        }

        robot = positionAdd(robot, direction);
    }

    uint64_t sum = 0;
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            if (getChar(&map,
                        (Position){
                                .x = x,
                                .y = y,
                        })
                != 'O') {
                continue;
            }

            sum += x + 100 * y;
        }
    }

    printf("%zu\n", sum);

    vector_move_delete(&moves);
    vector_string_each(&map, deleteString, NULL);
    vector_string_delete(&map);
    fclose(input);
    return 0;
}
