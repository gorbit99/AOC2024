#include "../utils/read_line.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
} Position;

#define VECTOR_NAME     PositionVector
#define VECTOR_TYPE     Position
#define VECTOR_FUN_NAME position
#include "../utils/vector.h"

size_t letterToIndex(char letter) {
    if (letter >= 'a' && letter <= 'z') {
        return letter - 'a';
    }
    if (letter >= 'A' && letter <= 'Z') {
        return 26 + letter - 'A';
    }
    return 52 + letter - '0';
}

#define ANTENNA_TYPE_COUNT 62

Position positionAdd(Position a, Position b) {
    return (Position){
            .x = a.x + b.x,
            .y = a.y + b.y,
    };
}

Position positionSub(Position a, Position b) {
    return (Position){
            .x = a.x - b.x,
            .y = a.y - b.y,
    };
}

int gcd(int a, int b) {
    a = abs(a);
    b = abs(b);
    while (a > 0) {
        int temp = b % a;
        b = a;
        a = temp;
    }
    return b;
}

bool isKnown(Position position, bool **knownAntinodes) {
    return knownAntinodes[position.y][position.x];
}

void markKnown(Position position, bool **knownAntinodes) {
    knownAntinodes[position.y][position.x] = true;
}

bool positionIsInBounds(Position a, size_t width, size_t height) {
    return a.x >= 0 && a.x < width && a.y >= 0 && a.y < height;
}

uint32_t countAntinodes(PositionVector *positions,
                        bool **knownAntinodes,
                        size_t width,
                        size_t height) {
    size_t positionCount = vector_position_size(positions);
    if (positionCount == 0) {
        return 0;
    }
    uint32_t uniqueCount = 0;
    for (size_t i = 0; i < positionCount - 1; i++) {
        Position positionA = vector_position_at(positions, i);
        for (size_t j = i + 1; j < positionCount; j++) {
            Position positionB = vector_position_at(positions, j);

            Position difference = positionSub(positionB, positionA);
            int divisor = gcd(difference.x, difference.y);
            difference.x /= divisor;
            difference.y /= divisor;

            Position antinode = positionA;
            while (positionIsInBounds(antinode, width, height)) {
                if (!isKnown(antinode, knownAntinodes)) {
                    uniqueCount++;
                    markKnown(antinode, knownAntinodes);
                }

                antinode = positionSub(antinode, difference);
            }

            antinode = positionAdd(positionA, difference);
            while (positionIsInBounds(antinode, width, height)) {
                if (!isKnown(antinode, knownAntinodes)) {
                    uniqueCount++;
                    markKnown(antinode, knownAntinodes);
                }

                antinode = positionAdd(antinode, difference);
            }
        }
    }

    return uniqueCount;
}

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "r");

    PositionVector positions[ANTENNA_TYPE_COUNT];
    for (size_t i = 0; i < ANTENNA_TYPE_COUNT; i++) {
        positions[i] = vector_position_new();
    }

    char *line;
    size_t y = 0;
    size_t width;
    while ((line = readLineFromFile(input, '\n')) != NULL) {
        width = strlen(line);
        for (size_t x = 0; x < width; x++) {
            if (line[x] == '.') {
                continue;
            }

            size_t index = letterToIndex(line[x]);
            vector_position_push(&positions[index],
                                 (Position){
                                         .x = x,
                                         .y = y,
                                 });
        }

        free(line);
        y++;
    }

    size_t height = y;

    bool **knownAntinodes = malloc(sizeof(bool *) * height);
    for (size_t i = 0; i < height; i++) {
        knownAntinodes[i] = calloc(width, sizeof(bool));
    }

    uint32_t uniqueCount = 0;
    for (size_t i = 0; i < ANTENNA_TYPE_COUNT; i++) {
        uniqueCount +=
                countAntinodes(&positions[i], knownAntinodes, width, height);
        vector_position_delete(&positions[i]);
    }

    printf("%u\n", uniqueCount);

    for (size_t i = 0; i < height; i++) {
        free(knownAntinodes[i]);
    }
    free(knownAntinodes);

    fclose(input);
    return 0;
}
