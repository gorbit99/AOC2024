#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *readLineFromFile(FILE *file, char delim) {
    size_t length = 0;

    uint32_t start = ftell(file);

    char c;
    while (fscanf(file, "%c", &c) != EOF) {
        if (c == delim && delim != -1) {
            break;
        }
        length++;
    }

    if (length == 0) {
        return NULL;
    }

    char *line = malloc(sizeof(char) * (length + 1));
    fseek(file, start, SEEK_SET);
    fread(line, sizeof(char), length, file);
    line[length] = '\0';

    if (c == delim && delim != -1) {
        fseek(file, 1, SEEK_CUR);
    }

    return line;
}
