// https://leetcode.com/problems/check-if-array-is-sorted-and-rotated
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    bool check(const vector<int> &nums) const
    {
        const auto it = ranges::is_sorted_until(nums);
        return it == nums.end()
               || (is_sorted(it, nums.end()) && nums.front() >= nums.back());
    }
};
