// https://leetcode.com/problems/range-sum-query-immutable/
//
// Caches the accumulative sums for each element of nums, enabling constant-time
// lookup. Getting the sum for a particular range is as easy as looking-up the
// accumulative sum for the end of the range & subtracting the sum **before**
// the beginning of the range. 0 is stored as the first cached result so that
// this works nicely if the range starts at the beginning of nums (this is why
// indexing accumSums_ is offset by 1).
//
// Complexity: lookup runtime O(1), pre-processing runtime O(n), space O(n).

class NumArray {
    vector<int> accumSums_;

public:
    NumArray(vector<int>& nums)
    {
        accumSums_.reserve(size(nums) + 1);
        accumSums_.push_back(0);
        partial_sum(begin(nums), end(nums), back_inserter(accumSums_));
    }

    int sumRange(int i, int j)
    {
        return accumSums_[j + 1] - accumSums_[i];
    }
};

/**
 * Your NumArray object will be instantiated and called as such:
 * NumArray* obj = new NumArray(nums);
 * int param_1 = obj->sumRange(i,j);
 */
