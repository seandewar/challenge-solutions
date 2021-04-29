// https://leetcode.com/problems/find-first-and-last-position-of-element-in-sorted-array
//
// Complexity: runtime O(logn), space O(1).

class Solution {
public:
    vector<int> searchRange(const vector<int>& nums, const int target) const
    {
        const auto [l, r] = equal_range(cbegin(nums), cend(nums), target);
        if (l == cend(nums) || *l != target)
            return {-1, -1};

        const int i = l - cbegin(nums), j = r - 1 - cbegin(nums);
        return {i, j};
    }
};
