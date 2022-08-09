// https://leetcode.com/problems/longest-increasing-subsequence
// Binary search approach; see comments for details.
// Complexity: runtime O(nlogn), space O(n).

class Solution {
public:
  int lengthOfLIS(const vector<int> &nums) const {
    vector<int> seq;
    for (const int x : nums) {
      if (const auto it = lower_bound(begin(seq), end(seq), x); it == end(seq)) {
        // Value is larger than the largest value in our longest subsequence.
        seq.push_back(x);
      } else {
        // This is smaller than the largest value in our longest subsequence, so
        // its size remains the same.
        // Instead, we want to start a new, smaller subsequence that ends with
        // this value as its largest.
        // As we only care about the length of the longest subsequence, and not
        // its contents, we can simply revise the value in our sequence at the
        // same index with no ill effects to the length of the longest
        // subsequence.
        // E.g: if we have two subsequences:
        // [0, 1, 2, 4, 5] and [0, 1, 2, 3]
        // This is represented in our sequence as:
        // [[0, 1, 2, 3], 5].
        *it = x;
      }
    }
    // The size always reflects that of our largest subsequence.
    return size(seq);
  }
};
