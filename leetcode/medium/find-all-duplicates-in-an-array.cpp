// https://leetcode.com/problems/find-all-duplicates-in-an-array/
//
// Marking already-seen numbers in-place.
//
// For each number x in the input, we mark the number at index (abs(x) - 1) as
// negative, indicating that we have now seen number x; if the marked number at
// index (abs(x) - 1) was already negative, we have already previously seen x,
// therefore x is duplicated in the input.
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    vector<int> findDuplicates(vector<int>& nums) const
    {
        vector<int> result;

        for (const int x : nums) {
            if ((nums[abs(x) - 1] *= -1) > 0) {
                result.push_back(abs(x));
            }
        }

        return result;
    }
};

// Alternative Solution: Continuously swap each number x with the number at
// index (x - 1), except if they are both equal, until all numbers x are at
// index (x - 1) in the input.
//
// Because we didn't swap equal xs, the duplicate copy of these xs will be not
// be in index (x - 1) in the modified input, thus they can be identified and
// returned.
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    vector<int> findDuplicates(vector<int>& nums) const
    {
        vector<int> result;

        for (int& x : nums) {
            while (x != nums[x - 1]) {
                swap(x, nums[x - 1]);
            }
        }

        for (size_t i = 0; i < size(nums); ++i) {
            if (nums[i] != i + 1) {
                result.push_back(nums[i]);
            }
        }

        return result;
    }
};
