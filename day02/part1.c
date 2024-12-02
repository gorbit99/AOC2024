#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "r");

    uint32_t safeCount = 0;

    int result = 2;
    while (result == 2) {
        uint32_t prevNum;
        uint32_t num;
        char c;
        bool firstNum = true;
        bool safe = true;
        bool diffFound = false;
        bool positiveDiff;

        while (true) {
            result = fscanf(input, "%d%c", &num, &c);

            if (!firstNum) {
                int diff = (int)num - (int)prevNum;
                if (abs(diff) < 1 || abs(diff) > 3) {
                    safe = false;
                }

                if (diffFound && (diff > 0) != positiveDiff) {
                    safe = false;
                }

                if (!diffFound) {
                    positiveDiff = diff > 0;
                    diffFound = true;
                }
            }

            prevNum = num;
            firstNum = false;

            if (c == '\n' || result != 2) {
                break;
            }
        }

        if (safe) {
            safeCount++;
        }
    }
    safeCount--;
    printf("%u\n", safeCount);

    return 0;
}
