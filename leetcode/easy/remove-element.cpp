// https://leetcode.com/problems/remove-element/
//
// Standard library solution. See alternative solution for less cheese.
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int removeElement(vector<int>& nums, const int val) const
    {
        return remove(begin(nums), end(nums), val) - cbegin(nums);
    }
};

// Alternative Solution: In-place removal is required. Uses 2 iterators.
// Complexity: runtime O(n), space O(1).
/*
class Solution {
public:
    int removeElement(vector<int>& nums, const int val) const
    {
        // refers to the new position of the val denoted by it in the new range
        // after the logical removal process. refers to the past-the-end pos in
        // the new list once finished
        auto newPosIt = begin(nums);

        for (auto it = cbegin(nums); it != cend(nums); ++it) {
            // for every val that we don't want to remove, overwrite the val at
            // newPosIt with the one at it. this has the effect of excluding
            // the values we want logically removed from the list (at it), as
            // newPosIt is not incremented when removal occurs
            if (*it != val)
                *(newPosIt++) = *it;
        }

        return newPosIt - cbegin(nums);
    }
};
*/
