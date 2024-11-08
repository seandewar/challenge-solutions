// https://leetcode.com/problems/maximum-xor-for-each-query
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    vector<int> getMaximumXor(const vector<int> &nums, const int max_bit) const
    {
        vector<int> ret(nums.size());
        for (int xorv = 0, i = 0; i < nums.size(); ++i) {
            xorv ^= nums[i];
            ret[ret.size() - 1 - i] = ((1 << max_bit) - 1) & ~xorv;
        }
        return ret;
    }
};
