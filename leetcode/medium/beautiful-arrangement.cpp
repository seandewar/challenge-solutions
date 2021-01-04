// https://leetcode.com/problems/beautiful-arrangement/
//
// Back-tracking solution.
//
// Attempt to generate every permutation via recursion & swapping. If we find
// that the sequence cannot be a "beautiful arrangement" at any step of
// permutation generation, prune that branch and back-track, as any permutation
// generated from that incomplete sequence cannot be "beautiful".
//
// Complexity: runtime usually less than O(n!), due to the pruning, space O(n)
// [recursion overhead].

class Solution {
    int countArrangementImpl(const int n, vector<int>& nums) const noexcept
    {
        if (n == 0)
            return 1;

        int result = 0;

        for (size_t i = 0; i < n; ++i) {
            if (nums[i] % n != 0 && n % nums[i] != 0)
                continue; // no perms with this prefix can be beautiful; prune

            swap(nums[i], nums[n - 1]);
            result += countArrangementImpl(n - 1, nums);
            swap(nums[i], nums[n - 1]); // back-track
        }

        return result;
    }

public:
    int countArrangement(const int n) const
    {
        vector<int> nums(n);
        iota(begin(nums), end(nums), 1);
        return countArrangementImpl(n, nums);
    }
};
