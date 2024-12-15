#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "r");

    int64_t width = 101;
    int64_t height = 103;

    uint64_t quadrants[4] = {0};

    int64_t rx;
    int64_t ry;
    int64_t vx;
    int64_t vy;
    for (int64_t step = 1; step < 1000; step++) {
        while (fscanf(input, "p=%ld,%ld v=%ld,%ld\n", &rx, &ry, &vx, &vy)
               != EOF) {
            rx = ((rx + vx * step) % width + width) % width;
            ry = ((ry + vy * step) % height + height) % height;

            size_t quadrantId = 0;
            if (rx == width / 2 || ry == height / 2) {
                continue;
            }
            if (rx < width / 2) {
                quadrantId += 1;
            }
            if (ry < height / 2) {
                quadrantId += 2;
            }
            quadrants[quadrantId]++;
        }
    }

    printf("%ld\n", quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3]);

    fclose(input);
    return 0;
}
