#include "../utils/read_line.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint64_t getChecksum(uint64_t *numbers,
                     size_t index,
                     size_t curPos,
                     uint64_t blockId) {
    uint64_t len = numbers[index];
    uint64_t startValue = curPos;
    uint64_t endValue = curPos + len - 1;
    return (startValue + endValue) * len / 2 * blockId;
}

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "r");
    char *line = readLineFromFile(input, '\n');

    size_t len = strlen(line);
    uint64_t *numbers = malloc(sizeof(uint64_t) * len);

    for (size_t i = 0; i < len; i++) {
        numbers[i] = line[i] - '0';
    }
    free(line);

    size_t firstIndex = 0;
    size_t lastIndex = len - 2 + len % 2;

    size_t curPos = 0;
    uint64_t checksum = 0;
    while (firstIndex < lastIndex) {
        checksum += getChecksum(numbers, firstIndex, curPos, firstIndex / 2);
        curPos += numbers[firstIndex];
        firstIndex++;

        while (numbers[firstIndex] > 0 && firstIndex < lastIndex) {
            if (numbers[lastIndex] <= numbers[firstIndex]) {
                checksum +=
                        getChecksum(numbers, lastIndex, curPos, lastIndex / 2);
                curPos += numbers[lastIndex];
                numbers[firstIndex] -= numbers[lastIndex];
                lastIndex -= 2;
            } else {
                checksum +=
                        getChecksum(numbers, firstIndex, curPos, lastIndex / 2);
                numbers[lastIndex] -= numbers[firstIndex];
                curPos += numbers[firstIndex];
                numbers[firstIndex] = 0;
            }
        }
        firstIndex++;
    }

    if (firstIndex == lastIndex) {
        checksum += getChecksum(numbers, firstIndex, curPos, firstIndex / 2);
    }

    printf("%lu\n", checksum);

    free(numbers);
    fclose(input);
}
