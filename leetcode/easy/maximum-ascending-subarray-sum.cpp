// https://leetcode.com/problems/maximum-ascending-subarray-sum
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int maxAscendingSum(const vector<int> &nums) const
    {
        int ret = 0;
        for (int sum = 0, prev = 0; const int x : nums) {
            if (x <= prev)
                sum = 0;
            sum += x;
            ret = max(ret, sum);
            prev = x;
        }
        return ret;
    }
};
