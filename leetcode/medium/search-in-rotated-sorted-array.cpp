// https://leetcode.com/problems/search-in-rotated-sorted-array
//
// The key is figuring out that we should also use the left and right pointers
// in our comparisons. Once that's known, you just need to translate the
// intuition of where to search next into code.
//
// Complexity: runtime O(logn), space O(1).

class Solution {
public:
    int search(const vector<int> &nums, const int target) const noexcept
    {
        for (int l = 0, r = size(nums) - 1; l <= r;) {
            const int m = l + (r - l) / 2;
            if (nums[m] == target)
                return m;
            if (nums[l] == target)
                return l;
            if (nums[r] == target)
                return r;
            if ((nums[l] < nums[m] && target > nums[l] && target < nums[m])
                || (nums[l] > nums[m]
                    && (target < nums[m] || target > nums[l])))
                r = m - 1;
            else
                l = m + 1;
        }
        return -1;
    }
};
