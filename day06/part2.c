#include "../utils/read_line.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define VECTOR_TYPE     char *
#define VECTOR_NAME     StringVector
#define VECTOR_FUN_NAME string
#include "../utils/vector.h"

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
} Direction;

typedef struct {
    int x;
    int y;
    Direction dir;
} GuardPosition;

bool isOutOfBounds(GuardPosition position, size_t width, size_t height) {
    return position.x < 0 || position.x >= width || position.y < 0
           || position.y >= height;
}

void setPosition(StringVector *map, int x, int y, char c) {
    vector_string_at(map, y)[x] = c;
}

char getPosition(StringVector *map, int x, int y) {
    return vector_string_at(map, y)[x];
}

bool isPreviouslySeen(char **previousPositions, GuardPosition position) {
    char value = previousPositions[position.y][position.x];
    return value & 1 << position.dir;
}

void markSeen(char **previousPositions, GuardPosition position) {
    previousPositions[position.y][position.x] |= 1 << position.dir;
}

bool isPreviouslySeenAny(char **previousPositions, GuardPosition position) {
    char value = previousPositions[position.y][position.x];
    return value != 0;
}

char **createPreviouslySeen(size_t width, size_t height) {
    char **seen = malloc(sizeof(char *) * height);
    for (size_t y = 0; y < height; y++) {
        seen[y] = calloc(width, sizeof(char));
    }
    return seen;
}

void freePreviouslySeen(char **previousPositions, size_t width, size_t height) {
    for (size_t y = 0; y < height; y++) {
        free(previousPositions[y]);
    }
    free(previousPositions);
}

char **dupPreviouslySeen(char **previousPositions,
                         size_t width,
                         size_t height) {
    char **seen = createPreviouslySeen(width, height);
    for (size_t y = 0; y < height; y++) {
        memcpy(seen[y], previousPositions[y], sizeof(char) * width);
    }
    return seen;
}

GuardPosition nextPosition(GuardPosition position) {
    GuardPosition result = {
            .x = position.x,
            .y = position.y,
            .dir = position.dir,
    };

    switch (position.dir) {
    case UP:
        result.y--;
        break;
    case DOWN:
        result.y++;
        break;
    case LEFT:
        result.x--;
        break;
    case RIGHT:
        result.x++;
        break;
    }

    return result;
}

Direction nextDirection(Direction current) {
    switch (current) {
    case UP:
        return RIGHT;
    case DOWN:
        return LEFT;
    case LEFT:
        return UP;
    case RIGHT:
        return DOWN;
    }
    return UP;
}

bool checkForLoops(StringVector *map,
                   GuardPosition position,
                   char **previousPositions,
                   size_t width,
                   size_t height) {
    char **seen = dupPreviouslySeen(previousPositions, width, height);

    while (!isOutOfBounds(position, width, height)) {
        markSeen(seen, position);
        GuardPosition nextPos = nextPosition(position);

        while (!isOutOfBounds(nextPos, width, height)
               && getPosition(map, nextPos.x, nextPos.y) == '#') {
            position.dir = nextDirection(position.dir);
            nextPos = nextPosition(position);
        }

        if (isOutOfBounds(nextPos, width, height)) {
            break;
        }

        position = nextPos;

        if (isPreviouslySeen(seen, position)) {
            freePreviouslySeen(seen, width, height);
            return true;
        }
    }

    freePreviouslySeen(seen, width, height);
    return false;
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

    GuardPosition guardPosition;
    size_t width = strlen(vector_string_at(&map, 0));
    size_t height = vector_string_size(&map);
    bool found = false;
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            if (getPosition(&map, x, y) == '^') {
                guardPosition.x = x;
                guardPosition.y = y;
                guardPosition.dir = UP;
                setPosition(&map, x, y, 'X');
                found = true;
                break;
            }
        }
        if (found) {
            break;
        }
    }

    char **previousPositions = createPreviouslySeen(width, height);

    uint32_t possibleObstructions = 0;
    while (guardPosition.x >= 0 && guardPosition.x < width
           && guardPosition.y >= 0 && guardPosition.y < height) {
        markSeen(previousPositions, guardPosition);
        GuardPosition nextPos = nextPosition(guardPosition);

        while (!isOutOfBounds(nextPos, width, height)
               && getPosition(&map, nextPos.x, nextPos.y) == '#') {
            guardPosition.dir = nextDirection(guardPosition.dir);
            nextPos = nextPosition(guardPosition);
        }

        if (isOutOfBounds(nextPos, width, height)) {
            break;
        }

        if (!isPreviouslySeenAny(previousPositions, nextPos)) {
            setPosition(&map, nextPos.x, nextPos.y, '#');
            if (checkForLoops(&map,
                              guardPosition,
                              previousPositions,
                              width,
                              height)) {
                possibleObstructions++;
            }
            setPosition(&map, nextPos.x, nextPos.y, '.');
        }

        guardPosition = nextPos;
    }

    printf("%d\n", possibleObstructions);

    freePreviouslySeen(previousPositions, width, height);

    vector_string_each(&map, deleteString, NULL);
    vector_string_delete(&map);

    fclose(input);
    return 0;
}
