// Bonus C99 solution, for fun! Reads from stdin. Assumes valid input.
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char name[16];
    uint_least32_t size;
    struct Node *children[32], **children_end;
};

static inline struct Node *node_new(struct Node **nodes_end, const char *name)
{
    strcpy((*nodes_end)->name, name);
    (*nodes_end)->size = 0;
    (*nodes_end)->children_end = (*nodes_end)->children;
    return (*nodes_end)++;
}

static inline void path_push(struct Node ***path_end, struct Node *node)
{
    **path_end = node;
    ++*path_end;
}

int main(void)
{
    struct Node nodes[1024], *nodes_end = nodes;
    struct Node *path[32], **path_end = path;
    path_push(&path_end, node_new(&nodes_end, "")); // insert root dir

    for (char line[64]; fgets(line, sizeof line, stdin) != NULL;) {
        char *nl = strchr(line, '\n');
        if (nl != NULL) {
            *nl = '\0';
        }

        struct Node *cwd = *(path_end - 1);
        char *size_end;
        if (strncmp(line, "$ cd ", 5) == 0) {
            char *dir_name = line + 5;
            if (strcmp(dir_name, "..") == 0) {
                --path_end;
                continue;
            }
            if (strcmp(dir_name, "/") == 0) {
                path_end = path + 1;
                continue;
            }
            for (struct Node **p = cwd->children;; ++p) {
                if (strcmp(dir_name, (*p)->name) == 0) {
                    path_push(&path_end, *p);
                    break;
                }
            }
        } else if (strncmp(line, "dir ", 4) == 0) {
            *cwd->children_end = node_new(&nodes_end, line + 4);
            ++cwd->children_end;
        } else {
            long long size = strtoll(line, &size_end, 10);
            if (size_end == line) {
                continue;
            }
            for (struct Node **p = path; p != path_end; ++p) {
                (*p)->size += size;
            }
        }
    }

    uint_least32_t p1 = 0, p2 = UINT_LEAST32_MAX;
    uint_least32_t p2_needed_space = 30000000 - (70000000 - nodes[0].size);
    for (const struct Node *p = nodes; p != nodes_end; ++p) {
        if (p->size <= 100000) {
            p1 += p->size;
        }
        if (p->size >= p2_needed_space && p->size < p2) {
            p2 = p->size;
        }
    }

    printf("Day7 (C99): P1: %" PRIuLEAST32 ", P2: %" PRIuLEAST32 "\n", p1, p2);
}
