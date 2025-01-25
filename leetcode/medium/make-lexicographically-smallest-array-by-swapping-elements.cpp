// https://leetcode.com/problems/make-lexicographically-smallest-array-by-swapping-elements
// Complexity: runtime O(nlogn), space O(n).

class Solution {
public:
    vector<int> lexicographicallySmallestArray(vector<int> &nums,
                                               const int limit) const
    {
        vector<int> sorted_is(nums.size()), ret(nums.size()), is;
        ranges::iota(sorted_is, 0);
        ranges::sort(sorted_is, [&](const int i, const int j) {
            return nums[i] < nums[j];
        });

        for (int l = 0; l < nums.size();) {
            is = {sorted_is[l]};
            for (int r = l;
                 r + 1 < nums.size()
                 && nums[sorted_is[r]] + limit >= nums[sorted_is[r + 1]];)
                is.push_back(sorted_is[++r]);

            ranges::sort(is);
            for (const int i : is)
                ret[i] = nums[sorted_is[l++]];
        }
        return ret;
    }
};
