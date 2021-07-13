// https://leetcode.com/problems/find-peak-element
//
// Binary search; we'll eventually end up at some sort of local or global peak.
// Complexity: runtime O(logn), space O(1).

class Solution {
public:
    int findPeakElement(const vector<int>& nums) const noexcept
    {
        size_t l = 0, r = size(nums) - 1;
        while (r > l) {
            const auto m = l + (r - l) / 2;
            if (nums[m] > nums[m + 1]) {
                r = m;
            } else {
                l = m + 1;
            }
        }
        return r;
    }
};

// Alternative Solution: More intuitive approach; checks both neighbours on each
// iteration.
// Complexity: runtime O(logn), space O(1).

class Solution {
public:
    int findPeakElement(const vector<int>& nums) const noexcept
    {
        size_t l = 0, r = size(nums) - 1;
        while (r >= l) {
            const auto m = l + (r - l) / 2;
            const bool llt = m == 0 || nums[m - 1] < nums[m];
            const bool rlt = m == size(nums) - 1 || nums[m] > nums[m + 1];
            if (llt && rlt) {
                return m;
            } else if (llt) {
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
        throw runtime_error("not found");
    }
};
