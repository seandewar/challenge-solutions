// https://leetcode.com/problems/intersection-of-two-arrays-ii/
//
// Complexity: runtime O(m+n), space O(m+n).

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int *intersect(int *const restrict a1, const int n1, int *const restrict a2,
               const int n2, int *const restrict retn)
{
    int counts[1001] = {0};
    for (int i = 0; i < n1; ++i) {
        ++counts[a1[i]];
    }
    const int maxn = n1 < n2 ? n1 : n2;
    int *const ret = malloc(maxn * sizeof(int)), j = 0;
    for (int i = 0; i < n2; ++i) {
        if (counts[a2[i]] > 0) {
            --counts[a2[i]];
            ret[j++] = a2[i];
        }
    }
    *retn = j;
    return ret;
}
