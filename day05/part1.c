#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint32_t first;
    uint32_t last;
} PageOrder;

#include "../utils/read_line.h"

#define VECTOR_NAME     PageOrderVector
#define VECTOR_TYPE     PageOrder
#define VECTOR_FUN_NAME pageorder
#include "../utils/vector.h"

#define VECTOR_NAME     Uint32Vector
#define VECTOR_TYPE     uint32_t
#define VECTOR_FUN_NAME uint32
#include "../utils/vector.h"

Uint32Vector pagesFromLine(char *line) {
    Uint32Vector pages = vector_uint32_new();

    size_t currentChar = 0;
    size_t lineLen = strlen(line);
    while (currentChar < lineLen) {
        int read = 0;
        uint32_t number;
        sscanf(line + currentChar, "%d%n", &number, &read);
        if (sscanf(line + currentChar, ",") != EOF) {
            currentChar++;
        }

        vector_uint32_push(&pages, number);

        currentChar += read;
    }

    return pages;
}

int pageComparator(uint32_t pageA, uint32_t pageB, PageOrderVector *orders) {
    size_t orderingRules = vector_pageorder_size(orders);

    for (size_t rule = 0; rule < orderingRules; rule++) {
        PageOrder order = vector_pageorder_at(orders, rule);

        if (order.first == pageA && order.last == pageB) {
            return -1;
        }

        if (order.first == pageB && order.last == pageA) {
            return 1;
        }
    }

    return 0;
}

bool isCorrectOrder(Uint32Vector *pages, PageOrderVector *orders) {
    size_t pageCount = vector_uint32_size(pages);

    for (size_t first = 0; first < pageCount - 1; first++) {
        for (size_t second = first + 1; second < pageCount; second++) {
            uint32_t firstPage = vector_uint32_at(pages, first);
            uint32_t secondPage = vector_uint32_at(pages, second);

            int correctOrder = pageComparator(firstPage, secondPage, orders);
            if (correctOrder == -1) {
                break;
            }
            if (correctOrder == 1) {
                return false;
            }
        }
    }
    return true;
}

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "r");

    PageOrderVector orders = vector_pageorder_new();

    char *line;
    PageOrder pageOrder;
    while ((line = readLineFromFile(input, '\n')) != NULL) {
        sscanf(line, "%d|%d", &pageOrder.first, &pageOrder.last);
        vector_pageorder_push(&orders, pageOrder);
        free(line);
    }

    uint32_t result = 0;

    while ((line = readLineFromFile(input, '\n')) != NULL) {
        Uint32Vector pages = pagesFromLine(line);
        size_t pageCount = vector_uint32_size(&pages);

        if (isCorrectOrder(&pages, &orders)) {
            result += vector_uint32_at(&pages, pageCount / 2);
        }

        vector_uint32_delete(&pages);
        free(line);
    }

    printf("%u\n", result);

    vector_pageorder_delete(&orders);
    fclose(input);
    return 0;
}
