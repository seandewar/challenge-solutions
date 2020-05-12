// https://leetcode.com/problems/single-element-in-a-sorted-array/
//
// Uses a binary search; the method is given away by the problem's O(logn)
// runtime and O(1) space requirement.
//
// We know that at any index before the point where the non-duplicate exists,
// each even index will contain the 1st of a duplicate number, and the odd
// index after will contain the 2nd. This solution performs a binary search
// while testing if the aforementioned property is still true, adjusting the
// search range to narrow in on where the non-duplicate element is.
//
// Complexity: runtime O(logn), space O(1).

class Solution {
public:
    int singleNonDuplicate(const vector<int>& nums) const
    {
        // can't use ssize(nums) on leetcode because compiler isn't
        // C++20-compliant, but it really doesn't matter here
        vector<int>::difference_type left = 0, right = size(nums) - 1;

        while (left < right) {
            // always guaranteeing an even middle means we don't need to
            // duplicate conditions in the if statement afterwards
            auto middle = left + (right - left) / 2;
            middle -= middle % 2;

            if (nums[middle] == nums[middle + 1])
                left = middle + 2;
            else
                right = middle - 1;
        }

        return nums[left]; // input always has an element that appears once
    }
};
