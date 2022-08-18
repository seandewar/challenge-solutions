// https://leetcode.com/problems/find-minimum-in-rotated-sorted-array
// Complexity: runtime O(logn), space O(1).

class Solution {
public:
    int findMin(const vector<int> &nums) const noexcept
    {
        int l = 0, r = size(nums) - 1;
        while (l < r) {
            int m = l + (r - l) / 2;
            if (nums[m] <= nums[r]) {
                r = m;
            } else {
                l = m + 1;
            }
        }
        return nums[l];
    }
};
