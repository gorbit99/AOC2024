#include <math.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "r");

    int64_t axPlus;
    int64_t ayPlus;
    int64_t bxPlus;
    int64_t byPlus;
    int64_t prizeX;
    int64_t prizeY;

    int64_t tokens = 0;

    while (fscanf(input,
                  "Button A: X+%lu, Y+%lu\nButton B: X+%lu, Y+%lu\nPrize: X=%lu, Y=%lu\n\n",
                  &axPlus,
                  &ayPlus,
                  &bxPlus,
                  &byPlus,
                  &prizeX,
                  &prizeY)
           == 6) {
        prizeX += 10000000000000;
        prizeY += 10000000000000;
        double n1 = (prizeY - prizeX / (double)bxPlus * byPlus)
                    / (ayPlus - axPlus / (double)bxPlus * byPlus);
        double n2 = (prizeX - n1 * axPlus) / bxPlus;

        if (fabs(round(n1) - n1) > 0.01 || fabs(round(n2) - n2) > 0.01) {
            continue;
        }

        n1 = round(n1);
        n2 = round(n2);

        tokens += n1 * 3 + n2;
    }

    printf("%lu\n", tokens);

    fclose(input);
    return 0;
}
