#include <stdio.h>
#include <stdlib.h>

char *readLineFromFile(FILE *file, char delim) {
    size_t length = 0;

    char c;
    while (fscanf(file, "%c", &c) != EOF) {
        if (c == '\n') {
            break;
        }
        length++;
    }

    if (length == 0) {
        return NULL;
    }

    char *line = malloc(sizeof(char) * (length + 1));
    fseek(file, -(int)length - 1, SEEK_CUR);
    fread(line, sizeof(char), length, file);
    line[length] = '\0';

    if (c == '\n') {
        fseek(file, 1, SEEK_CUR);
    }

    return line;
}
