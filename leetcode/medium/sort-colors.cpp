// https://leetcode.com/problems/sort-colors/
//
// The sorting can be done in-place, using constant space and in one pass of the
// input fairly easily. We're not allowed to use library functions to sort.
//
// See code comments for more details.
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    void sortColors(vector<int>& nums) const
    {
        vector<int>::size_type reds = 0, whites = 0, blues = 0;

        for (const int colour : nums) {
            if (colour == 0) { // red
                // reds come first, so we will need to shift blues and whites to
                // the right to make room for our red. we need only shift the
                // first blue in our sorted range to the new end of our sorted
                // range, with the first white taking its shifted-from place

                nums[reds + whites + blues] = 2;
                nums[reds + whites] = 1;
                nums[reds++] = 0;
            } else if (colour == 1) { // white
                // similar idea as before, but because whites come after reds,
                // we don't need to do any shifting of the reds, so only shift
                // the blues as before

                nums[reds + whites + blues] = 2;
                nums[reds + whites++] = 1;
            } else { // blue
                // finally, because blues come after reds and whites, we don't
                // need to do any shifting at all; the colour is already in the
                // right place for our sorted range (at the current end of it)

                ++blues;
            }
        }
    }
};
