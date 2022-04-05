/* https://leetcode.com/problems/pascals-triangle */
/*  */
/* Complexity: runtime O(n^2), space O(1) (ignoring output). */

/**
 * Return an array of arrays of size *rm.
 * The sizes of the arrays are returned as *rns array.
 * Note: Both returned array and *rns array must be malloced, assume caller
 * calls free().
 */
int **generate(const int n, int *const restrict rm, int *restrict *const rns)
{
    int **const ret = malloc(n * sizeof *ret);
    *rm = n;
    *rns = malloc(n * sizeof **rns);
    for (int i = 0; i < n; ++i) {
        const int len = i + 1;
        (*rns)[i] = len;
        ret[i] = malloc(len * sizeof ret[i][0]);
        ret[i][len - 1] = ret[i][0] = 1;
        for (int j = 1; j < len - 1; ++j) {
            ret[i][j] = ret[i - 1][j - 1] + ret[i - 1][j];
        }
    }
    return ret;
}
