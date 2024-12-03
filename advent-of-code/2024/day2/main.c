#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static unsigned find_mismatches(const char *const line, const bool asc,
                                const bool skip_first)
{
    unsigned mismatches = skip_first ? 1 : 0;

    long prev_num;
    const char *p = line;
    char *next;
    for (unsigned i = 0;; p = next, ++i) {
        const long num = strtol(p, &next, 10);
        if (errno != 0 || p == next) {
            if (errno == ERANGE)
                abort(); // overflow

            for (; isspace(*p); ++p) {}
            if (*p != '\0' || (p != line && *(p - 1) != '\n'))
                abort(); // invalid number or line too long
            if (i == 0)
                return UINT_MAX; // empty line; treat as invalid for both parts
            break;
        }
        if (skip_first && i == 0)
            continue;

        if (i > (skip_first ? 1 : 0)
            && (num == prev_num || num > prev_num != asc
                || labs(num - prev_num) > 3)) {
            if (++mismatches > 1)
                break;
            continue;
        }

        prev_num = num;
    }

    return mismatches;
}

int main(void)
{
    FILE *const file = fopen("input", "r");
    if (!file)
        abort();

    unsigned p1 = 0, p2 = 0;
    for (char line_buf[1024]; fgets(line_buf, sizeof line_buf, file);) {
        const unsigned mismatches[] = {
            find_mismatches(line_buf, false, false),
            find_mismatches(line_buf, true, false),
            find_mismatches(line_buf, false, true),
            find_mismatches(line_buf, true, true),
        };
        unsigned min_mismatches = mismatches[0];
        for (unsigned i = 1; i < 4; ++i)
            if (mismatches[i] < min_mismatches)
                min_mismatches = mismatches[i];

        if (min_mismatches == 0)
            ++p1;
        if (min_mismatches < 2)
            ++p2;
    }
    if (!feof(file))
        abort();
    fclose(file);

    printf("Day2: P1: %u, P2: %u\n", p1, p2);
}
