/* https://leetcode.com/problems/search-a-2d-matrix */
/*  */
/* Complexity: runtime O(logn + logm), space O(1). */

int cmp(const void *const a, const void *const b)
{
    return *(int *)a - *(int *)b;
}

bool searchMatrix(const int *const restrict *const mat, const int m,
                  const int *const restrict ns, const int target)
{
    int l = 0, r = m - 1;
    while (l <= r) {
        const int i = l + (r - l) / 2;
        if (mat[i][0] == target) {
            return true;
        } else if (mat[i][0] > target) {
            r = i - 1;
        } else {
            l = i + 1;
        }
    }
    l = l > 0 ? --l : 0;
    return bsearch(&target, mat[l], ns[l], sizeof(int), cmp);
}
