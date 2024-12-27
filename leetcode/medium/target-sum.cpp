// https://leetcode.com/problems/target-sum
//
// A more efficient solution is totally possible via DP, but the constraints
// don't require it (1 <= n <= 20).
//
// Complexity: runtime O(2^n), space O(n).

class Solution {
public:
    int findTargetSumWays(const vector<int> &nums, const int target)
    {
        int ret = 0;
        const auto dfs = [&](this const auto &self, const int sum,
                             const int i) {
            if (i >= nums.size()) {
                ret += sum == target;
                return;
            }
            self(sum + nums[i], i + 1);
            self(sum - nums[i], i + 1);
        };
        dfs(0, 0);
        return ret;
    }
};
