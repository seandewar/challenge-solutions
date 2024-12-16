#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *const file = fopen("input", "r");
    if (!file)
        abort(); // open failed

    int width = 0, height = 0;
    {
        int x = 0;
        for (char c; (c = getc(file)) != EOF;) {
            if (c == '\n') {
                ++height;
                if (width <= 0)
                    width = x;
                else if (x != width)
                    abort(); // line width differs
                x = 0;
                continue;
            }
            ++x;
        }
        if (!feof(file))
            abort(); // read error
        if (x > 0) {
            ++height;
            if (width <= 0)
                width = x;
            else if (x != width)
                abort(); // line width differs
        }
        rewind(file);
    }

    const int dirs[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    int(*best_scores)[4] = malloc(width * height * sizeof best_scores[0]);
    if (!best_scores)
        abort(); // oom
#define BEST_SCORE(x, y, dir_i) best_scores[(y) * width + (x)][(dir_i)]

    int start_x = -1, start_y = -1;
    int end_x = -1, end_y = -1;
    {
        int x = 0, y = 0;
        for (char c; (c = getc(file)) != EOF;) {
            if (c == '\n') {
                ++y;
                x = 0;
                continue;
            }
            if (c == 'S') {
                if (start_x != -1)
                    abort(); // multiple starts
                start_x = x;
                start_y = y;
            } else if (c == 'E') {
                if (end_x != -1)
                    abort(); // multiple ends
                end_x = x;
                end_y = y;
            }

            for (int i = 0; i < 4; ++i)
                BEST_SCORE(x, y, i) = c == '#' ? INT_MIN : INT_MAX;
            ++x;
        }
        if (!feof(file) || start_x == -1 || end_x == -1)
            abort(); // read error or no start/end
        fclose(file);
    }

    struct Walk {
        int x, y, dir_i;
        int score;
    };
    int walk_queue_len = 0;
    struct Walk *const walk_queue = // implicit min heap on score for P1
        malloc(4 * width * height * sizeof walk_queue[0]);
    if (!walk_queue)
        abort(); // oom

#define WALK_HEAP_PUSH(...)                                            \
    do {                                                               \
        const struct Walk walk_ = {__VA_ARGS__};                       \
        if (BEST_SCORE(walk_.x, walk_.y, walk_.dir_i) <= walk_.score)  \
            break;                                                     \
                                                                       \
        walk_queue[walk_queue_len] = (const struct Walk){__VA_ARGS__}; \
        for (int i_ = walk_queue_len++; i_ > 0;) {                     \
            const int parent_i_ = i_ / 2;                              \
            if (walk_queue[parent_i_].score <= walk_queue[i_].score)   \
                break;                                                 \
            const struct Walk parent_ = walk_queue[parent_i_];         \
            walk_queue[parent_i_] = walk_queue[i_];                    \
            walk_queue[i_] = parent_;                                  \
            i_ = parent_i_;                                            \
        }                                                              \
    } while (false)

    int p1 = -1;
    WALK_HEAP_PUSH(.x = start_x, .y = start_y);
    while (walk_queue_len-- > 0) {
        const struct Walk top = walk_queue[0];
        for (int i = 0;;) { // pop heap
            const int l_i = 2 * i + 1, r_i = 2 * i + 2;
            const int l_score = l_i < walk_queue_len ? walk_queue[l_i].score
                                                     : INT_MAX,
                      r_score = r_i < walk_queue_len ? walk_queue[r_i].score
                                                     : INT_MAX,
                      min_score = l_score < r_score ? l_score : r_score;
            if (walk_queue[walk_queue_len].score <= min_score) {
                walk_queue[i] = walk_queue[walk_queue_len];
                break;
            }
            const int next_i = walk_queue[l_i].score == min_score ? l_i : r_i;
            walk_queue[i] = walk_queue[next_i];
            i = next_i;
        }

        if (p1 != -1 && top.score > p1)
            break; // no more best paths remain for P2

        if (BEST_SCORE(top.x, top.y, top.dir_i) <= top.score)
            continue;
        BEST_SCORE(top.x, top.y, top.dir_i) = top.score;

        if (top.x == end_x && top.y == end_y) {
            if (p1 == -1) {
                // found a best path to the end; don't terminate immediately
                // though, as there may be other equally good paths for P2...
                p1 = top.score;
                printf("Day16: P1: %d", p1);
                fflush(stdout);
            }
            continue;
        }
        for (int i = 0; i < 4; ++i)
            WALK_HEAP_PUSH(.x = top.x + dirs[i][0], .y = top.y + dirs[i][1],
                           .dir_i = i,
                           .score = top.score + 1 + 1000 * (top.dir_i != i));
    }
    if (p1 == -1)
        abort(); // end unreachable

#undef WALK_HEAP_PUSH
    walk_queue_len = 0; // used as a stack for P2; "dir_i" field is left unused
#define WALK_STACK_PUSH(...) \
    (walk_queue[walk_queue_len++] = (const struct Walk){__VA_ARGS__})

    bool *const p2_counted = malloc(width * height * sizeof p2_counted[0]);
    if (!p2_counted)
        abort();
    for (int i = 0; i < width * height; ++i)
        p2_counted[i] = false;
#define P2_COUNTED(x, y) p2_counted[(y) * width + (x)]

    int p2 = 0;
    WALK_STACK_PUSH(.x = end_x, .y = end_y, .score = p1);
    while (walk_queue_len-- > 0) {
        const struct Walk top = walk_queue[walk_queue_len];
        if (!P2_COUNTED(top.x, top.y)) {
            ++p2;
            P2_COUNTED(top.x, top.y) = true;
        }

        for (int i = 0; i < 4; ++i) {
            const int x2 = top.x - dirs[i][0], y2 = top.y - dirs[i][1];
            for (int i2 = 0; i2 < 4; ++i2) {
                const int other_score = BEST_SCORE(x2, y2, i2);
                if (other_score == top.score - 1 - 1000 * (i != i2))
                    WALK_STACK_PUSH(.x = x2, .y = y2, .score = other_score);
            }
        }
    }
    printf(", P2: %d\n", p2);

#undef P2_COUNTED
    free(p2_counted);
#undef WALK_STACK
    free(walk_queue);
#undef BEST_SCORE
    free(best_scores);
}
