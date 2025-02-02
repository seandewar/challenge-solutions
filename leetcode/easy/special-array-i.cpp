// https://leetcode.com/problems/special-array-i
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    bool isArraySpecial(const vector<int> &nums) const
    {
        for (int i = 1; i < nums.size(); ++i)
            if (nums[i] % 2 == nums[i - 1] % 2)
                return false;
        return true;
    }
};
