// https://leetcode.com/problems/find-the-power-of-k-size-subarrays-i
// Complexity: runtime O(n), space O(n-k) including output, O(1) otherwise.

class Solution {
public:
    vector<int> resultsArray(const vector<int> &nums, const int k) const
    {
        vector<int> ret;
        ret.reserve(nums.size() + 1 - k);
        for (int i = 0, seqn = 0; i < nums.size(); ++i) {
            if (i == 0 || nums[i] == nums[i - 1] + 1)
                ++seqn;
            else
                seqn = 1;
            if (i >= k - 1)
                ret.push_back(seqn >= k ? nums[i] : -1);
        }
        return ret;
    }
};
