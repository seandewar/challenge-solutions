#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    FILE *input = fopen("input", "r");
    if (!input) {
        perror("fopen");
        return EXIT_FAILURE;
    }

#define UPDATE_TOP3                                                      \
    do {                                                                 \
        size_t _min_i = top3[0] < top3[1] ? (top3[0] < top3[2] ? 0 : 2)  \
                                          : (top3[1] < top3[2] ? 1 : 2); \
        if (top3[_min_i] < accum) {                                      \
            top3[_min_i] = accum;                                        \
        }                                                                \
    } while (0)

    long top3[3] = {0};
    long accum = 0;
    for (char line[16]; fgets(line, sizeof line, input);) {
        if (!strchr(line, '\n')) {
            fputs("fgets eof or line too long\n", stderr);
            goto fail;
        }
        if (line[0] == '\n') {
            UPDATE_TOP3;
            accum = 0;
            continue;
        }

        char *endptr;
        accum += strtol(line, &endptr, 10);
        if (errno != 0) {
            perror("strtol");
            goto fail;
        }
        if (*endptr != '\n') {
            fprintf(stderr, "strtol invalid character: %c\n", *endptr);
            goto fail;
        }
    }
    UPDATE_TOP3;
    fclose(input);

    int p1 = top3[top3[0] > top3[1] ? (top3[0] > top3[2] ? 0 : 2)
                                    : (top3[1] > top3[2] ? 1 : 2)];
    int p2 = top3[0] + top3[1] + top3[2];
    printf("Day1 (C): P1: %d, P2: %d\n", p1, p2);
    return EXIT_SUCCESS;

fail:
    fclose(input);
    return EXIT_FAILURE;
}
