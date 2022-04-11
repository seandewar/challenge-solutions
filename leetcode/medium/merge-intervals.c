/* https://leetcode.com/problems/merge-intervals */
/* ~44ms -- faster than 100%! */
/* Complexity: runtime O(nlogn), space O(logn) [O(n) if counting output]. */

int cmp(const void *const restrict a, const void *const restrict b)
{
    const int *x = *(int **)a, *y = *(int **)b;
    return x[0] == y[0] ? y[1] - x[1] : x[0] - y[0];
}

int **merge(const int *restrict *const restrict inters, const int m,
            int *const restrict ns, int *const restrict retm,
            int *restrict *const restrict retns)
{
    qsort((void *)inters, m, sizeof(int **), cmp);
    int **ret = malloc(m * sizeof(int *));
    *retm = 0;

    int curr[2] = {inters[0][0], inters[0][1]};
#define PUSH_CURRENT                      \
    ret[*retm] = malloc(2 * sizeof(int)); \
    ret[*retm][0] = curr[0];              \
    ret[(*retm)++][1] = curr[1]

    for (int i = 1; i < m; ++i) {
        if (inters[i][0] == inters[i - 1][0]) // upper bound bisect is better
            continue;

        if (inters[i][0] <= curr[1]) {
            if (curr[1] < inters[i][1])
                curr[1] = inters[i][1];
            continue;
        }

        PUSH_CURRENT;
        curr[0] = inters[i][0];
        curr[1] = inters[i][1];
    }

    PUSH_CURRENT;
    ret = realloc(ret, *retm * sizeof(int *));
    *retns = malloc(*retm * sizeof(int));
    for (int i = 0; i < *retm; ++i)
        (*retns)[i] = 2;

    return ret;
}
