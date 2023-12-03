#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

#define GRID_SIZE 140
#define GRID_NONE 0
#define GRID_ASTERISK -1
#define GRID_SYMBOL -2
static short grid[GRID_SIZE][GRID_SIZE];

static short grid_get(const int x, const int y) {
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE ? grid[y][x] : GRID_NONE;
}

static bool grid_has_part(const int x, const int y) {
    return grid_get(x, y) < GRID_NONE;
}

static bool grid_has_num_minmax(const int x, const int y, short *const restrict min,
                                short *const restrict max) {
    const short v = grid_get(x, y);
    if (v <= GRID_NONE)
        return false;

    *min = v < *min ? v : *min;
    *max = v > *max ? v : *max;
    return true;
}

int main(void) {
    FILE *const f = fopen("input", "r");
    assert(f);
    for (int x = 0, y = 0, c; (c = getc(f)) != EOF;) {
        if (c == '\n') {
            ++y;
            x = 0;
            continue;
        }

        assert(x < GRID_SIZE && y < GRID_SIZE && c != '0');
        if (c < '0' || c > '9') {
            grid[y][x++] = c == '.' ? GRID_NONE : (c == '*' ? GRID_ASTERISK : GRID_SYMBOL);
            continue;
        }

        short v = c - '0';
        int x2 = x + 1;
        for (; (c = getc(f)) != EOF && c >= '0' && c <= '9'; ++x2)
            v = (v * 10) + c - '0';

        const int pushed = ungetc(c, f);
        assert(pushed == c && x2 <= GRID_SIZE);
        for (; x < x2; ++x)
            grid[y][x] = v;
    }
    assert(feof(f));
    fclose(f);

    int p1 = 0, p2 = 0;
    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE;) {
            const short v = grid[y][x];

            if (v > GRID_NONE) {
                bool has_adj = grid_has_part(x - 1, y - 1) || grid_has_part(x - 1, y)
                               || grid_has_part(x - 1, y + 1);

                for (; x < GRID_SIZE && grid[y][x] == v; ++x)
                    has_adj = has_adj || grid_has_part(x, y - 1) || grid_has_part(x, y + 1);

                has_adj = has_adj || grid_has_part(x, y - 1) || grid_has_part(x, y)
                          || grid_has_part(x, y + 1);

                if (has_adj)
                    p1 += v;
                continue;
            }

            if (v == GRID_ASTERISK) {
                short min_adj = SHRT_MAX, max_adj = GRID_NONE;
#define HAS_NUM(x, y) grid_has_num_minmax((x), (y), &min_adj, &max_adj)

                int adj_count = HAS_NUM(x - 1, y) + HAS_NUM(x + 1, y);

                if (HAS_NUM(x - 1, y - 1) && !HAS_NUM(x, y - 1) && HAS_NUM(x + 1, y - 1))
                    adj_count += 2;
                else if (HAS_NUM(x - 1, y - 1) || HAS_NUM(x, y - 1) || HAS_NUM(x + 1, y - 1))
                    ++adj_count;

                if (HAS_NUM(x - 1, y + 1) && !HAS_NUM(x, y + 1) && HAS_NUM(x + 1, y + 1))
                    adj_count += 2;
                else if (HAS_NUM(x - 1, y + 1) || HAS_NUM(x, y + 1) || HAS_NUM(x + 1, y + 1))
                    ++adj_count;
#undef HAS_NUM

                if (adj_count == 2)
                    p2 += min_adj * max_adj;
            }

            ++x;
        }
    }

    printf("Day3: P1: %d, P2: %d\n", p1, p2);
}
