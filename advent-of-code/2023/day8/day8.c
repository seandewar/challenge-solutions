// This puzzle is rather lame. The general solution for P2 is quite hard, so
// (not wanting to spend all day working on it) I made the assumption that this
// can be solved by finding the periods of the ghosts' cycles ending at the **Z
// node, and finding the LCM, which seems to work for my input.
//
// This means the input likely has the following properties for each ghost:
//
// - there is a cycle that contains a **Z node.
// - there is only one such **Z node in that cycle.
// - reaching that **Z node always takes the same number of moves, so the period
//   can be found by reaching it once.
//
// These conditions would require *a lot* of luck to satisfy by random chance
// for *all* ghosts, so it's likely the inputs are generated to always meet
// those conditions, and that this is the intended way to solve the problem.
//
// However, the problem statement does not mention these constraints at all!
//
// Cringe, but AoC has pulled such tricks in the past many times; this is why,
// if a problem seems very hard (especially on day 8!), assume a hidden
// constraint exists that can greatly simplify the problem, and examine the
// input to confirm it exists (unless you want the extra challenge for fun, like
// with my general 2022 day 22 solution).

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char name[4], left[4], right[4];
};

static int node_compare(const void *const lhs, const void *const rhs) {
    return strcmp(((const struct Node *)lhs)->name,
                  ((const struct Node *)rhs)->name);
}

int main(void) {
    FILE *const f = fopen("input", "r");
    assert(f);

    size_t move_count = 0, node_count = 0;
    char c, dummy[4];
    for (; (c = getc(f)) == 'L' || c == 'R'; ++move_count) {}
    for (int matched;
         (matched = fscanf(f, "%*3s = (%*3s ,%3s )", dummy)) != EOF;
         ++node_count) {
        assert(matched == 1);
    }

    char *const moves = malloc(move_count);
    struct Node *const nodes = malloc(node_count * sizeof nodes[0]);
    assert(moves && nodes);

    const int err = fseek(f, 0, SEEK_SET);
    assert(err == 0);
    const size_t read_count = fread(moves, 1, move_count, f);
    assert(read_count == move_count);
    for (size_t i = 0; i < node_count; ++i) {
        const int matched = fscanf(f, "%3s = (%3s ,%3s )", nodes[i].name,
                                   nodes[i].left, nodes[i].right);
        assert(matched == 3);
    }
    fclose(f);

    qsort(nodes, node_count, sizeof nodes[0], node_compare);
    const bool do_p1 = node_count > 0 && strcmp(nodes[0].name, "AAA") == 0;

    size_t p2 = 0;
    for (size_t i = 0; i < node_count; ++i) {
        const struct Node *node = &nodes[i];
        if (node->name[2] != 'A')
            continue;

        size_t move = 0;
        for (; node->name[2] != 'Z'; ++move) {
            node = bsearch(moves[move % move_count] == 'L' ? node->left
                                                           : node->right,
                           nodes, node_count, sizeof nodes[0], node_compare);
            assert(node);
        }

        if (p2 == 0) {
            if (do_p1) {
                printf("Day8: P1: %zu", move);
                fflush(stdout);
            }
            p2 = move;
            continue;
        }

        size_t a = p2, b = move;
        while (a != b) {
            if (a > b)
                b += move;
            else
                a += p2;
            assert(a >= p2 && b >= move);
        }
        p2 = a;
    }

    printf("%sP2: %zu\n", do_p1 ? ", " : "Day8: ", p2);
    free(nodes);
    free(moves);
}
