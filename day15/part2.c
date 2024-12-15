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

#define VECTOR_TYPE     Position
#define VECTOR_NAME     PositionVector
#define VECTOR_FUN_NAME position
#include "../utils/vector.h"

typedef enum {
    LEFT = '<',
    RIGHT = '>',
    UP = '^',
    DOWN = 'v',
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

PositionVector getPushPositions(StringVector *map,
                                Position robot,
                                Position direction,
                                size_t width,
                                size_t height) {
    PositionVector pushes = vector_position_new();
    PositionVector stack = vector_position_new();

    bool **seen = malloc(sizeof(bool *) * height);
    for (size_t y = 0; y < height; y++) {
        seen[y] = calloc(width, sizeof(bool));
    }

    vector_position_push(&stack, robot);
    while (vector_position_size(&stack) > 0) {
        Position check = vector_position_shift(&stack);
        if (seen[check.y][check.x]) {
            continue;
        }
        seen[check.y][check.x] = true;

        char checkChar = getChar(map, check);

        if (checkChar == '#') {
            vector_position_delete(&pushes);
            vector_position_delete(&stack);
            for (size_t y = 0; y < height; y++) {
                free(seen[y]);
            }
            free(seen);
            return vector_position_new();
        }

        if (checkChar == '.') {
            continue;
        }

        if (checkChar == '@') {
            vector_position_push(&pushes, check);
            Position nextPosition = positionAdd(check, direction);
            vector_position_push(&stack, nextPosition);
        }

        if (checkChar == '[' || checkChar == ']') {
            Position otherSide;
            if (checkChar == '[') {
                otherSide = positionAdd(check, (Position){.x = 1, .y = 0});
            } else {
                otherSide = positionAdd(check, (Position){.x = -1, .y = 0});
            }
            vector_position_push(&pushes, check);
            vector_position_push(&pushes, otherSide);
            seen[otherSide.y][otherSide.x] = true;

            if (direction.x != 0) {
                Position nextPosition = positionAdd(otherSide, direction);
                vector_position_push(&stack, nextPosition);
            } else {
                Position nextPosition1 = positionAdd(check, direction);
                Position nextPosition2 = positionAdd(otherSide, direction);
                vector_position_push(&stack, nextPosition1);
                vector_position_push(&stack, nextPosition2);
            }
        }
    }

    for (size_t y = 0; y < height; y++) {
        free(seen[y]);
    }
    free(seen);
    vector_position_delete(&stack);
    return pushes;
}

void deleteString(char **string, size_t _1, void *_2) {
    free(*string);
}

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "r");

    StringVector map = vector_string_new();

    char *line;
    while ((line = readLineFromFile(input, '\n')) != NULL) {
        size_t lineLength = strlen(line);
        char *newLine = malloc(sizeof(char) * (lineLength * 2 + 1));
        newLine[lineLength * 2] = '\0';
        for (size_t i = 0; line[i] != '\0'; i++) {
            switch (line[i]) {
            case '#':
                newLine[i * 2] = '#';
                newLine[i * 2 + 1] = '#';
                break;
            case 'O':
                newLine[i * 2] = '[';
                newLine[i * 2 + 1] = ']';
                break;
            case '.':
                newLine[i * 2] = '.';
                newLine[i * 2 + 1] = '.';
                break;
            case '@':
                newLine[i * 2] = '@';
                newLine[i * 2 + 1] = '.';
                break;
            }
        }
        vector_string_push(&map, newLine);
        free(line);
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

        PositionVector pushes =
                getPushPositions(&map, robot, direction, width, height);

        if (vector_position_size(&pushes) == 0) {
            vector_position_delete(&pushes);
            continue;
        }

        while (vector_position_size(&pushes) > 0) {
            Position push = vector_position_pop(&pushes);
            Position nextPosition = positionAdd(push, direction);
            setChar(&map, nextPosition, getChar(&map, push));
            setChar(&map, push, '.');
        }

        robot = positionAdd(robot, direction);
        vector_position_delete(&pushes);
    }

    uint64_t sum = 0;
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            if (getChar(&map,
                        (Position){
                                .x = x,
                                .y = y,
                        })
                != '[') {
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
