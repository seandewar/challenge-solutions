// https://leetcode.com/problems/find-if-array-can-be-sorted
//
// Can reduce this to O(nlogn) runtime via creating a sorted interval list of
// ranges with the same popcount (and probably even better runtime via other
// approaches), but this passed.
//
// Complexity: runtime O(n^2), space O(n).

class Solution {
public:
  bool canSortArray(const vector<int> &nums) const {
    auto sorted = nums;
    sort(sorted.begin(), sorted.end());
    for (size_t i = 0; i < nums.size(); ++i) {
      const size_t dst_i =
          lower_bound(sorted.begin(), sorted.end(), nums[i]) - sorted.begin();
      for (size_t j = min(dst_i, i); j < max(dst_i, i); ++j) {
        if (popcount<unsigned>(nums[j]) != popcount<unsigned>(nums[j + 1]))
          return false;
      }
    }
    return true;
  }
};
