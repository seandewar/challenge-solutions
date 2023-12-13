// For P2, I was tempted to just increase the resolution of the input to make
// pipes at least 2x2, and then solve it that way, but that felt icky,
// especially as we know which pipes are part of the main loop after P1.
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

struct Cell {
    char c;
    int visit_i;
};

#define GRID_SIZE 512
static struct Cell grid[GRID_SIZE][GRID_SIZE];

static struct Cell *grid_get(const int x, const int y) {
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE ? &grid[y][x]
                                                              : NULL;
}

int main(void) {
    FILE *const f = fopen("input", "r");
    assert(f);
    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x)
            grid[y][x].visit_i = -1;
    }
    int start_x = -1, start_y;
    for (int x = 0, y = 0, c; (c = getc(f)) != EOF;) {
        if (c == '\n') {
            x = 0;
            ++y;
            continue;
        }
        assert(x < GRID_SIZE && y < GRID_SIZE);
        if (c == 'S') {
            start_x = x;
            start_y = y;
        }
        grid[y][x++].c = c;
    }
    fclose(f);
    assert(start_x != -1);

    struct Cell *const l = grid_get(start_x - 1, start_y),
                       *const r = grid_get(start_x + 1, start_y),
                       *const u = grid_get(start_x, start_y - 1),
                       *const d = grid_get(start_x, start_y + 1);
    const bool w = l && (l->c == '-' || l->c == 'F' || l->c == 'L'),
               e = r && (r->c == '-' || r->c == '7' || r->c == 'J'),
               n = u && (u->c == '|' || u->c == 'F' || u->c == '7'),
               s = d && (d->c == '|' || d->c == 'L' || d->c == 'J');
    assert((w || e || n || s) && w + e + n + s < 3);
    int dx = 0, dy = 0;
    if (w && e) {
        grid[start_y][start_x].c = '-';
        dx = 1;
    } else if (n && s) {
        grid[start_y][start_x].c = '|';
        dy = 1;
    } else if (n) {
        grid[start_y][start_x].c = w ? 'J' : 'L';
        dy = -1;
    } else {
        grid[start_y][start_x].c = w ? '7' : 'F';
        dx = w ? -1 : 1;
    }

    int i = 0;
    for (int x = start_x, y = start_y; x != start_x || y != start_y || i == 0;
         ++i) {
        assert(grid[y][x].visit_i == -1);
        grid[y][x].visit_i = i;
        x += dx;
        y += dy;
        const char c = grid[y][x].c;
        if (c != '-' && c != '|') {
            if (dy == 0) {
                dx = 0;
                dy = c == 'F' || c == '7' ? 1 : -1;
            } else {
                dx = c == 'F' || c == 'L' ? 1 : -1;
                dy = 0;
            }
        }
    }
    assert(i % 2 == 0);
    printf("Day10: P1: %d", i / 2);
    fflush(stdout);

    int p2 = 0;
    for (int y = 0; y < GRID_SIZE && grid[y][0].c != 0; ++y) {
        bool inside = false;
        for (int x = 0; x < GRID_SIZE && grid[y][x].c != 0; ++x) {
            if (grid[y][x].visit_i < 0) {
                if (inside)
                    ++p2;
                continue;
            }
            const char c = grid[y][x].c;
            if (c == '|' || c == 'F' || c == '7')
                inside = !inside;
        }
    }
    printf(", P2: %d\n", p2);
}
