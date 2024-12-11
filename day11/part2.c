#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint64_t value;
    uint64_t count;
} Entry;

#define VECTOR_NAME     EntryVector
#define VECTOR_TYPE     Entry
#define VECTOR_FUN_NAME entry
#include "../utils/vector.h"

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1], "r");

    EntryVector entries = vector_entry_new();

    uint64_t number;
    while (fscanf(input, "%lu", &number) == 1) {
        vector_entry_push(&entries,
                          (Entry){
                                  .count = 1,
                                  .value = number,
                          });
    }

    size_t repeats = 75;
    EntryVector newEntries = vector_entry_new();
    for (size_t i = 0; i < repeats; i++) {
        while (vector_entry_size(&entries) > 0) {
            Entry entry = vector_entry_pop(&entries);
            Entry replaced[2];
            size_t newEntryCount;
            int digits = floor(log10(entry.value)) + 1;
            if (entry.value == 0) {
                entry.value = 1;
                replaced[0] = entry;
                newEntryCount = 1;
            } else if (digits % 2 == 0) {
                int divisor = pow(10, (float)digits / 2);
                uint64_t left = entry.value % divisor;
                uint64_t right = entry.value / divisor;

                replaced[0] = (Entry){.value = left, .count = entry.count};
                replaced[1] = (Entry){.value = right, .count = entry.count};
                newEntryCount = 2;
            } else {
                entry.value *= 2024;
                replaced[0] = entry;
                newEntryCount = 1;
            }

            for (size_t newEntry = 0; newEntry < newEntryCount; newEntry++) {
                bool found = false;
                for (size_t search = 0; search < vector_entry_size(&newEntries);
                     search++) {
                    if (vector_entry_at(&newEntries, search).value
                        == replaced[newEntry].value) {
                        vector_entry_at_ptr(&newEntries, search)->count +=
                                replaced[newEntry].count;
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    vector_entry_push(&newEntries, replaced[newEntry]);
                }
            }
        }

        EntryVector temp = entries;
        entries = newEntries;
        newEntries = temp;
    }

    uint64_t sum = 0;
    for (size_t i = 0; i < vector_entry_size(&entries); i++) {
        sum += vector_entry_at(&entries, i).count;
    }

    printf("%lu\n", sum);

    vector_entry_delete(&entries);
    vector_entry_delete(&newEntries);
    fclose(input);
}
