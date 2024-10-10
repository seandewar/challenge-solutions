// https://leetcode.com/problems/maximum-width-ramp
// Complexity: runtime O(n), space O(n).

int maxWidthRamp(const int *const nums, const int n)
{
    int istack[5 * 10000], *top = istack;
    for (int i = 0; i < n; ++i) {
        if (top == istack || nums[i] < nums[*(top - 1)])
            *(top++) = i;
    }
    int ret = 0;
    for (int j = n - 1; j > 0 && top > istack;) {
        if (nums[j] >= nums[*(top - 1)]) {
            if (ret < j - *(--top))
                ret = j - *top;
            continue;
        }
        --j;
    }
    return ret;
}

// Alternative Solution: before I simplified to use a stack.
// Complexity: runtime O(n+m), space O(m). [where m is the range of numbers
//                                          allowed, which is really the same as
//                                          n, so same complexity as previous]

int maxWidthRamp(const int *const nums, const int n)
{
#define BESTJ_SIZE (5 * 10000 + 1)
    int bestj[BESTJ_SIZE];
    for (int i = 0; i < BESTJ_SIZE; ++i)
        bestj[i] = -1;
    for (int i = 0; i < n; ++i)
        bestj[nums[i]] = i;

    int maxj = -1;
    for (int i = BESTJ_SIZE - 1; i >= 0; --i) {
        if (bestj[i] > maxj)
            maxj = bestj[i];
        bestj[i] = maxj;
    }

    int ret = 0;
    for (int i = 0; i < n - 1; ++i) {
        const int dist = bestj[nums[i]] - i;
        if (dist > ret)
            ret = dist;
    }
    return ret;
}
