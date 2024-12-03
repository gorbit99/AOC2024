#include "../utils/read_line.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "r");

    char *line = readLineFromFile(input, -1);
    char *current = line;
    uint32_t sum = 0;
    bool mulEnabled = true;

    while (current != NULL) {
        char *nextMul = strstr(current, "mul(");
        char *nextDo = strstr(current, "do()");
        char *nextDont = strstr(current, "don't()");
        current = NULL;

        if (nextMul != NULL && (nextDo == NULL || nextMul < nextDo)
            && (nextDont == NULL || nextMul < nextDont)) {
            uint32_t a;
            uint32_t b;

            char end;
            if (sscanf(nextMul + 4, "%d,%d%c", &a, &b, &end) != 3
                || end != ')') {
                current = nextMul + 1;
                continue;
            }

            if (mulEnabled) {
                sum += a * b;
            }
            current = nextMul + 1;
            continue;
        }

        if (nextDo != NULL && (nextDont == NULL || nextDo < nextDont)) {
            mulEnabled = true;
            current = nextDo + 1;
            continue;
        }

        if (nextDont != NULL) {
            mulEnabled = false;
            current = nextDont + 1;
            continue;
        }
    }

    printf("%u\n", sum);
    free(line);
    fclose(input);

    return 0;
}
