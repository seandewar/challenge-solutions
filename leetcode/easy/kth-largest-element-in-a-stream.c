/* https://leetcode.com/problems/kth-largest-element-in-a-stream */
/* Uses a min heap, implemented using C's flexible array members. */
/*
 * Complexity: runtime: init O(n*logn), add O(logn),
 *             space O(k) [maybe O(k+logn) due to qsort; depends on its impl]
 */

typedef struct {
    int k;
    int heap[]; // flexible array member (l33t h4x) used as a min heap
} KthLargest;

int cmp(const void *const restrict a, const void *const restrict b)
{
    return *(int *)a - *(int *)b;
}

KthLargest *kthLargestCreate(const int k, int *const nums, const int n)
{
    qsort(nums, n, sizeof(int), cmp); // could use an O(n) heapify instead
    KthLargest *const ret = malloc(sizeof(KthLargest) + k * sizeof(int));
    ret->k = k;

    const int filler = n < k ? k - n : 0;
    memcpy(ret->heap + filler, filler ? nums : nums + n - k,
           (filler ? n : k) * sizeof(int));
    for (int i = 0; i < filler; ++i)
        ret->heap[i] = INT_MIN;

    return ret;
}

int kthLargestAdd(KthLargest *const obj, const int val)
{
    if (val <= obj->heap[0])
        return obj->heap[0];

    obj->heap[0] = val;
    for (int i = 0; 2 * i + 1 < obj->k;) {
        const int l = 2 * i + 1, r = l + 1;
        if (obj->heap[l] >= obj->heap[i]
            && (r >= obj->k || obj->heap[r] >= obj->heap[i]))
            break;

        const int next = r >= obj->k || obj->heap[l] < obj->heap[r] ? l : r;
        const int tmp = obj->heap[i];
        obj->heap[i] = obj->heap[next];
        obj->heap[next] = tmp;
        i = next;
    }

    return obj->heap[0];
}

void kthLargestFree(KthLargest *const obj) { free(obj); }

/**
 * Your KthLargest struct will be instantiated and called as such:
 * KthLargest* obj = kthLargestCreate(k, nums, numsSize);
 * int param_1 = kthLargestAdd(obj, val);

 * kthLargestFree(obj);
*/
