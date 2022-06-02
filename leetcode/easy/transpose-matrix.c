/* https://leetcode.com/problems/transpose-matrix */
/* Complexity: runtime O(m*n), space O(m*n) including output, O(1) otherwise. */

int **transpose(const int *const *const restrict mat, const int matn,
                const int *const restrict matms, int *const restrict retn,
                int **const restrict retms)
{
    *retn = matms[0];
    *retms = malloc(*retn * sizeof **retms);
    for (int i = 0; i < *retn; ++i) {
        (*retms)[i] = matn;
    }

    int **const ret = malloc(*retn * sizeof *ret);
    for (int y = 0; y < *retn; ++y) {
        ret[y] = malloc((*retms)[0] * sizeof **ret);
        for (int x = 0; x < (*retms)[0]; ++x) {
            ret[y][x] = mat[x][y];
        }
    }

    return ret;
}
