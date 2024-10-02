// https://leetcode.com/problems/rank-transform-of-an-array
// Complexity: runtime O(nlogn), space O(logn) [O(n) if including result].

static int cmp(const void *const a, const void *const b)
{
    return *(const int *)a - *(const int *)b;
}

int *arrayRankTransform(int *const arr, const int arrSize, int *const retSize)
{
    int *const ret = malloc(arrSize * sizeof ret[0]);
    memcpy(ret, arr, arrSize * sizeof arr[0]);
    qsort(ret, arrSize, sizeof ret[0], cmp);
    *retSize = 0;
    for (int i = 0; i < arrSize; ++i) {
        if (i == 0 || ret[i] != ret[i - 1])
            ret[(*retSize)++] = ret[i];
    }
    for (int i = 0; i < arrSize; ++i) {
        const int *const p =
            bsearch(&arr[i], ret, *retSize, sizeof ret[0], cmp);
        arr[i] = 1 + p - ret;
    }
    *retSize = arrSize;
    memcpy(ret, arr, arrSize * sizeof arr[0]);
    return ret;
}
