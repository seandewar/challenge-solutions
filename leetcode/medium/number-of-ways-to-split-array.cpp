// https://leetcode.com/problems/number-of-ways-to-split-array
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int waysToSplitArray(const vector<int> &nums) const
    {
        int ret = 0;
        auto prefix = 0ll, suffix = accumulate(nums.begin(), nums.end(), 0ll);
        for (int i = 0; i < nums.size() - 1; ++i) {
            prefix += nums[i];
            suffix -= nums[i];
            ret += prefix >= suffix;
        }
        return ret;
    }
};
