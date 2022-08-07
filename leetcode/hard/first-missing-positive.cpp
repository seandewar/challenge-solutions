// https://leetcode.com/problems/first-missing-positive
//
// Re-order and swap elements so that x is at index x - 1, but only if x > 0.
// Also, watch out for infinite swapping loops if duplicates exist!
//
// To find the smallest missing positive integer, loop a 2nd time from the
// beginning and find x where its index is not x - 1.
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int firstMissingPositive(vector<int> &nums) const noexcept
    {
        for (int i = 0; i < size(nums);) {
            if (nums[i] <= 0 || nums[i] > size(nums) || nums[i] == i + 1
                || nums[i] == nums[nums[i] - 1]) {
                ++i;
                continue;
            }
            swap(nums[i], nums[nums[i] - 1]);
        }
        for (int i = 0; i < size(nums); ++i) {
            if (nums[i] != i + 1)
                return i + 1;
        }
        return size(nums) + 1;
    }
};
