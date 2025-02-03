// https://leetcode.com/problems/longest-strictly-increasing-or-strictly-decreasing-subarray
// Using only one counter is possible, but this is simple.
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int longestMonotonicSubarray(const vector<int>& nums) const {
        int ret = 1;
        for (int i = 1, inc_len = 1, dec_len = 1; i < nums.size(); ++i) {
            inc_len = nums[i] >= nums[i - 1] ? 1 : inc_len + 1;
            dec_len = nums[i] <= nums[i - 1] ? 1 : dec_len + 1;
            ret = max(ret, max(inc_len, dec_len));
        }
        return ret;
    }
};
