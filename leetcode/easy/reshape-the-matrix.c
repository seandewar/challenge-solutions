// https://leetcode.com/problems/reshape-the-matrix
//
// Complexity: runtime O(m*n), space O(m*n).

/// Return an array of arrays of size *retm.
/// The sizes of the arrays are returned as *retns array.
/// Note: Both returned array and *ns array must be malloced, assume caller
///       calls free().
int **matrixReshape(const int *const restrict *const restrict mat, const int m,
                    const int *const restrict ns, int r, int c,
                    int *const restrict retm,
                    int *restrict *const restrict retns)
{
    const int n = ns[0];
    if (m * n != r * c) {
        r = m;
        c = n;
    }
    int **const ret = malloc(r * sizeof(int *));
    *retm = r;
    *retns = malloc(r * sizeof(int));
    for (int i = 0; i < r; ++i) {
        ret[i] = malloc(c * sizeof(int));
        (*retns)[i] = c;
    }
    for (int i = 0; i < m * n; ++i) {
        ret[i / c][i % c] = mat[i / n][i % n];
    }
    return ret;
}
