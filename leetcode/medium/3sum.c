/* https://leetcode.com/problems/3sum */
/* Complexity: runtime O(n^2), space likely O(logn) [from qsort]. */

int cmp(const void *const restrict a, const void *const restrict b)
{
    return *(int *)a - *(int *)b;
}

/**
 * Return an array of arrays of size *retm.
 * The sizes of the arrays are returned as *retns array.
 * Note: Both returned array and *columnSizes array must be malloced, assume
 * caller calls free().
 */
int **threeSum(int *const restrict nums, const int n, int *const restrict retm,
               int *restrict *const retns)
{
    int counts[100001 * 2] = {0}; // overflow stack for great benefit
#define COUNTS(i) (counts[(i) + 100001])

    for (int i = 0; i < n; ++i)
        ++COUNTS(nums[i]);

    qsort(nums, n, sizeof(int), cmp);
    int retcap = 16, **ret = malloc(retcap * sizeof(int *));
    *retm = 0;

    for (int i = 0; i < n; ++i) {
        if (i > 0 && nums[i] == nums[i - 1]) // upper bound bisect is better
            continue;

        for (int j = i + 1; j < n; ++j) {
            if (j > i + 1 && nums[j] == nums[j - 1]) // ditto
                continue;

            const int need = -nums[i] - nums[j];
            if (need < nums[j] || need > 100000
                || COUNTS(need) <= (nums[i] == need) + (nums[j] == need))
                continue;

            if (*retm >= retcap)
                ret = realloc(ret, (retcap *= 2) * sizeof(int *));

            ret[*retm] = malloc(3 * sizeof(int));
            ret[*retm][0] = nums[i];
            ret[*retm][1] = nums[j];
            ret[(*retm)++][2] = need;
        }
    }

    ret = realloc(ret, (*retm ? *retm : 1) * sizeof(int *));
    *retns = malloc(*retm * sizeof(int));
    for (int i = 0; i < *retm; ++i)
        (*retns)[i] = 3;

    return ret;
}
