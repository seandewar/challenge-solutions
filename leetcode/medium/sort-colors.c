/* https://leetcode.com/problems/sort-colors */
/* Complexity: runtime O(n), space O(1). */

void sortColors(int *const nums, const int n)
{
    int counts[3] = {0};
    for (int i = 0; i < n; ++i)
        ++counts[nums[i]];

    for (int i = 0, j = 0; i < n; ++i) {
        while (--counts[j] < 0)
            ++j;

        nums[i] = j;
    }
}
