// https://leetcode.com/problems/squares-of-a-sorted-array
// This solution meets the follow-up's requirement of running in linear time.
// Complexity: runtime O(n), space O(n) [O(1) if excluding output].

class Solution {
public:
    vector<int> sortedSquares(const vector<int> &nums) const
    {
        vector<int> ret;
        ret.reserve(nums.size());
        for (auto it1 = lower_bound(nums.begin(), nums.end(), 0), it2 = it1;
             it1 != nums.end() || it2 != nums.begin();) {
            const int l = it1 != nums.end() ? *it1 : numeric_limits<int>::max(),
                      r = it2 != nums.begin() ? abs(*(it2 - 1))
                                              : numeric_limits<int>::max();
            if (l <= r) {
                ret.push_back(l * l);
                ++it1;
            } else {
                --it2;
                ret.push_back(r * r);
            }
        }
        return ret;
    }
};
