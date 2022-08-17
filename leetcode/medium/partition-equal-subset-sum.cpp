// https://leetcode.com/problems/partition-equal-subset-sum
// Top-down DP

class Solution {
    bool dfs(const vector<int> &nums, const int rem, const int i,
             array<vector<bool>, 200> &mem) const
    {
        if (rem == 0) {
            return true;
        }
        if (rem < 0 || i >= size(nums) || (size(mem[i]) > rem && mem[i][rem])) {
            return false;
        }
        if (dfs(nums, rem - nums[i], i + 1, mem)
            || dfs(nums, rem, i + 1, mem)) {
            return true;
        }
        mem[i].resize(20'001);
        mem[i][rem] = true;
        return false;
    }

public:
    bool canPartition(const vector<int> &nums) const
    {
        const int tot = accumulate(begin(nums), end(nums), 0);
        if (tot % 2 != 0) {
            return false;
        }
        array<vector<bool>, 200> mem;
        return dfs(nums, tot / 2, 0, mem);
    }
};
