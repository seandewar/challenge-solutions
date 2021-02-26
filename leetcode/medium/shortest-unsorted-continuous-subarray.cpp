// https://leetcode.com/problems/shortest-unsorted-continuous-subarray
//
// Complexity: runtime O(n*logn), space O(n).

class Solution {
public:
    int findUnsortedSubarray(const vector<int>& nums) const
    {
        auto sortedNums = nums;
        sort(begin(sortedNums), end(sortedNums));

        const auto leftIt =
            mismatch(cbegin(nums), cend(nums), cbegin(sortedNums)).second;

        if (leftIt == cend(sortedNums))
            return 0; // no mismatch (nums is already sorted)

        const auto rightIt =
            mismatch(crbegin(nums), crend(nums), crbegin(sortedNums)).second;

        // due to how reverse_iterators are implemented on-top of regular
        // bi-directional iters, this expression returns the distance between
        // both iters, plus 1 (which is what we want anyway)
        return rightIt.base() - leftIt;
    }
};
