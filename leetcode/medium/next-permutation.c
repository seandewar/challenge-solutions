/* https://leetcode.com/problems/next-permutation */
/* See code comments for details. */
/* Complexity: runtime O(n), space O(1). */

void nextPermutation(int *const nums, const int n)
{
    // find first decreasing number from the right
    int i = n - 2;
    for (; i >= 0 && nums[i] >= nums[i + 1]; --i);

    if (i >= 0) {
        // find the number just larger than it to the right
        // (could use a binary search here instead (upper bound))
        int swapi = i + 1;
        for (; swapi < n && nums[swapi] > nums[i]; ++swapi);

        // swap these two numbers
        const int v = nums[i];
        nums[i] = nums[--swapi];
        nums[swapi] = v;
    }

    // reverse numbers to the right of where first decreasing number was
    for (int j = 1; j <= (n - i - 1) / 2; ++j) {
        const int v = nums[i + j];
        nums[i + j] = nums[n - j];
        nums[n - j] = v;
    }
}
