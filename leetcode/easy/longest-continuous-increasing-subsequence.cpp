// https://leetcode.com/problems/longest-continuous-increasing-subsequence
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int findLengthOfLCIS(const vector<int> &nums) const noexcept
    {
        int ret = 1;
        for (int i = 1, n = 1; i < size(nums); ++i) {
            if (nums[i] > nums[i - 1]) {
                ret = max(ret, ++n);
            } else {
                n = 1;
            }
        }
        return ret;
    }
};
