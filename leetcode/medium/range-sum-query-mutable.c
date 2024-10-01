// https://leetcode.com/problems/range-sum-query-mutable
//
// Segment tree (not to be confused with the computational geometry DS of the
// same name) based solution (via an array-based implicit DS).
//
// I always struggle with remembering how to implement Fenwick trees; segment
// trees feel more intuitive in comparison, though more effort to write.
//
// Complexity: runtime: create O(n), update O(logn), query O(logn); space: O(n).

typedef struct {
    int size, nodes[]; // flexible array memb for the nodes
} NumArray;

static int buildTree(NumArray *const t, const int ni, const int l,
                     const int len, const int *const nums)
{
    if (len <= 0)
        return 0;
    if (len == 1)
        return t->nodes[ni] = nums[l];

    const int m = l + len / 2, lni = 2 * ni + 1, rni = lni + 1;
    return t->nodes[ni] = buildTree(t, lni, l, len / 2, nums)
                          + buildTree(t, rni, m, len - len / 2, nums);
}

NumArray *numArrayCreate(const int *const nums, const int size)
{
    NumArray *const t = malloc(sizeof *t + (4 * size + 1) * sizeof t->nodes[0]);
    t->size = size;
    buildTree(t, 0, 0, size, nums);
    return t;
}

static int update(NumArray *const t, const int ni, const int l, const int len,
                  const int i, const int v)
{
    if (len <= 0)
        return 0;
    if (i < l || i >= l + len)
        return t->nodes[ni];
    if (len == 1)
        return t->nodes[ni] = v;

    const int m = l + len / 2, lni = 2 * ni + 1, rni = lni + 1;
    return t->nodes[ni] = update(t, lni, l, len / 2, i, v)
                          + update(t, rni, m, len - len / 2, i, v);
}

void numArrayUpdate(NumArray *const obj, const int index, const int val)
{
    update(obj, 0, 0, obj->size, index, val);
}

static int getSum(NumArray *const t, const int ni, const int l, const int len,
                  const int wantl, const int wantr)
{
    if (len <= 0)
        return 0;
    if (wantl <= l && wantr >= l + len - 1)
        return t->nodes[ni];

    const int m = l + len / 2, lni = 2 * ni + 1, rni = lni + 1;
    int sum = 0;
    if (wantl <= m - 1)
        sum += getSum(t, lni, l, len / 2, wantl, wantr);
    if (wantr >= m)
        sum += getSum(t, rni, m, len - len / 2, wantl, wantr);
    return sum;
}

int numArraySumRange(const NumArray *const obj, const int left, const int right)
{
    return getSum(obj, 0, 0, obj->size, left, right);
}

void numArrayFree(NumArray *const obj)
{
    free(obj);
}
