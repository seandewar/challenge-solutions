// https://leetcode.com/problems/rotate-array/ (right rotate)
//
// Solution uses C++ std, O(n) runtime, O(1) space. See
// https://en.cppreference.com/w/cpp/algorithm/rotate for details on its
// in-place implementation.
//
// Alternative solutions:
//
// Using reverses in-place: O(n) runtime, O(1) space:
//
// reverse(begin(nums), end(nums) - k % size(nums));
// reverse(end(nums) - k % size(nums), end(nums));
// reverse(begin(nums), end(nums));
//
// Using another array: O(n) runtime, O(n) space:
//
// auto old = nums;
// for (int i = 0; i < size(old); ++i) {
//     nums[(i + k) % size(old)] = old[i];
// }

class Solution {
public:
    void rotate(vector<int>& nums, int k)
    {
        std::rotate(rbegin(nums), rbegin(nums) + k % size(nums), rend(nums));
    }
};
