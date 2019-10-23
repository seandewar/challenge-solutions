// https://leetcode.com/problems/maximum-subarray/
//
// Keeps track of the current subarray sum and highest subarray sum found; the
// subarray must be at least size 1, so these values are initialized to the
// first element. When iterating, the current sum is combined with the current
// element's value only if the sum would be larger than that value, otherwise
// we reset the sum to the element's value (representing a new subarray
// starting here). Max sum is updated after every iteration to reflect the
// highest sum we've seen.
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int maxSubArray(vector<int>& nums)
    {
        if (empty(nums)) {
            return 0;
        }

        int currSum = nums.front();
        int currMax = currSum;

        for (auto it = cbegin(nums) + 1; it != cend(nums); ++it) {
            currSum = max(currSum + *it, *it);
            currMax = max(currMax, currSum);
        }

        return currMax;
    }
};
