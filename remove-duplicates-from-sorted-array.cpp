// https://leetcode.com/problems/remove-duplicates-from-sorted-array/
//
// Solution below is more-or-less equivalent to:
//
// // vector::size_type is more correct, but function returns int
// int newSize = 0;
// for (int i = 0; i < size(nums); ++i) {
//     if (i == 0 || nums[i - 1] != nums[i]) {
//         nums[newSize++] = nums[i];
//     }
// }
// return newSize;
//
// The solution can be changed to resize the resulting vector to the new size
// as so:
//
// nums.erase(unique(begin(nums), end(nums)), end(nums));
// return size(nums);
//
// However, this is slower (unless the size is equivalent) as it's similar to
// resize()ing the vector to the new, lesser size, which destructs all objects
// outside the new range - O(n) complexity.  (This could technically be
// completely optimized away (O(1)) if the objects are trivially destructable,
// but the standard does not mandate this for resize() to smaller N or erase()
// from M to N.

class Solution {
public:
    int removeDuplicates(vector<int>& nums)
    {
        return distance(begin(nums), unique(begin(nums), end(nums)));
    }
};
