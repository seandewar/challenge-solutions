// https://leetcode.com/problems/maximum-number-of-non-overlapping-subarrays-with-sum-equals-target
// Complexity: runtime O(n), space O(n).

class Solution {
public:
    int maxNonOverlapping(const vector<int> &nums, const int target) const
    {
        unordered_set ps{0};
        int ret = 0;
        for (int pre = 0; const int x : nums) {
            pre += x;
            if (ps.contains(pre - target)) {
                ++ret;
                ps.clear();
            }
            ps.insert(pre);
        }
        return ret;
    }
};
