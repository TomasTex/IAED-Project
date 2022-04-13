#include "sort.h"

#include <stdlib.h>
#include <string.h>

void bsort(void* array, const int n, unsigned long size, int (*cmp) (const void*, const void*)) {
    int i, j;
    void* tmp = malloc(size);

    for (i = 0; i < n - 1; ++i) {
        for (j = 0; j < n - i - 1; ++j) {
            void* a = ((char*) array) + (size * j);
            void* b = ((char*) array) + (size * (j + 1));

            if ((*cmp)(a, b) > 0) {
                memcpy(tmp, a, size);
                memcpy(a, b, size);
                memcpy(b, tmp, size);
            }
        }
    }

    free(tmp);
}
