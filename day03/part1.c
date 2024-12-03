#include "../utils/read_line.h"

#include <stdint.h>
#include <stdio.h>

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "r");

    char *line = readLineFromFile(input, -1);
    char *current = strstr(line, "mul(");
    uint32_t sum = 0;

    while (current != NULL) {
        uint32_t a;
        uint32_t b;

        char end;
        if (sscanf(current + 4, "%d,%d%c", &a, &b, &end) != 3 || end != ')') {
            current = strstr(current + 1, "mul(");
            continue;
        }

        sum += a * b;
        current = strstr(current + 1, "mul(");
        continue;
    }

    printf("%u\n", sum);
    free(line);
    fclose(input);

    return 0;
}
