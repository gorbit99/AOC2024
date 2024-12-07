#include "../utils/read_line.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define VECTOR_NAME     Uint64Vector
#define VECTOR_TYPE     uint64_t
#define VECTOR_FUN_NAME uint64
#include "../utils/vector.h"

typedef struct {
    size_t index;
    uint64_t total;
} BacktrackState;

#define VECTOR_NAME     BSVector
#define VECTOR_TYPE     BacktrackState
#define VECTOR_FUN_NAME bs
#include "../utils/vector.h"

bool checkIfPossible(Uint64Vector *values, uint64_t result) {
    size_t valueCount = vector_uint64_size(values);

    BSVector stack = vector_bs_new();
    vector_bs_push(&stack,
                   (BacktrackState){
                           .index = 1,
                           .total = vector_uint64_at(values, 0),
                   });

    while (vector_bs_size(&stack) > 0) {
        BacktrackState current = vector_bs_pop(&stack);
        if (current.index == valueCount) {
            if (current.total == result) {
                vector_bs_delete(&stack);
                return true;
            }
            continue;
        }

        uint64_t nextValue = vector_uint64_at(values, current.index);

        vector_bs_push(&stack,
                       (BacktrackState){
                               .index = current.index + 1,
                               .total = current.total + nextValue,
                       });
        vector_bs_push(&stack,
                       (BacktrackState){
                               .index = current.index + 1,
                               .total = current.total * nextValue,
                       });
        int toShift = floor(log10(nextValue)) + 1;
        vector_bs_push(
                &stack,
                (BacktrackState){
                        .index = current.index + 1,
                        .total = current.total * pow(10, toShift) + nextValue,
                });
    }

    vector_bs_delete(&stack);
    return false;
}

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "r");

    char *line;
    uint64_t total = 0;
    while ((line = readLineFromFile(input, '\n')) != NULL) {
        uint64_t result;
        sscanf(line, "%lu", &result);

        Uint64Vector values = vector_uint64_new();
        char *valueStart = strchr(line, ' ') + 1;

        uint64_t value;
        int read;
        while (sscanf(valueStart, "%lu%n", &value, &read) == 1) {
            valueStart += read;
            vector_uint64_push(&values, value);
        }

        if (checkIfPossible(&values, result)) {
            total += result;
        }

        vector_uint64_delete(&values);
        free(line);
    }

    printf("%lu\n", total);

    fclose(input);
}
