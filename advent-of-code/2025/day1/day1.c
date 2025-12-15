// Use a C23 compiler. Uses the new ckd_* functions for literally no reason.

#include <limits.h>
#include <stdckdint.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *const f = fopen("input", "r");
    if (!f)
        abort();

    // CBA to deal with negative modulos, so just orient ourselves somewhere
    // near the middle of the unsigned int range :-)
    constexpr unsigned orient = (UINT_MAX / 2) - ((UINT_MAX / 2) % 100);
    unsigned num = orient + 50;
    unsigned p1 = 0, p2 = 0;

    for (char buf[32]; fgets(buf, sizeof buf, f);) {
        char dir;
        unsigned turns;
        if (sscanf(buf, "%c%u ", &dir, &turns) != 2)
            abort();

        const unsigned prev_num = num;
        switch (dir) {
        case 'L':
            if (ckd_sub(&num, num, turns))
                abort();
            if (ckd_add(&p2, p2, ((prev_num - 1) / 100) - ((num - 1) / 100)))
                abort();
            break;
        case 'R':
            if (ckd_add(&num, num, turns))
                abort();
            if (ckd_add(&p2, p2, (num / 100) - (prev_num / 100)))
                abort();
            break;
        default:
            abort();
        }

        if (ckd_add(&p1, p1, (num % 100) == 0))
            abort();
        num = orient + (num % 100);
    }

    printf("Day1: P1: %u, P2: %u\n", p1, p2);
}
