#include "../utils/read_line.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Block {
    uint64_t id;
    size_t start;
    size_t len;
} Block;

uint64_t blockChecksum(Block block) {
    uint64_t endValue = block.start + block.len - 1;
    return block.id * (block.start + endValue) * block.len / 2;
}

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "r");
    char *line = readLineFromFile(input, '\n');

    size_t len = strlen(line);
    Block *blocks = malloc(sizeof(Block) * len);

    size_t curPos = 0;
    for (size_t i = 0; i < len; i++) {
        blocks[i] = (Block){
                .id = i / 2,
                .len = line[i] - '0',
                .start = curPos,
        };
        curPos += blocks[i].len;
    }
    free(line);

    uint64_t checksum = 0;
    size_t lastIndex = len - 2 + len % 2;
    for (int64_t cur = lastIndex; cur > 0; cur -= 2) {
        bool found = false;
        for (size_t check = 1; check < cur; check += 2) {
            if (blocks[check].len < blocks[cur].len) {
                continue;
            }

            blocks[cur].start = blocks[check].start;
            blocks[check].start += blocks[cur].len;
            blocks[check].len -= blocks[cur].len;
            checksum += blockChecksum(blocks[cur]);
            found = true;
            break;
        }

        if (!found) {
            checksum += blockChecksum(blocks[cur]);
        }
    }

    printf("%lu\n", checksum);
    free(blocks);
    fclose(input);
}
