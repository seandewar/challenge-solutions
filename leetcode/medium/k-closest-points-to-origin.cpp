// https://leetcode.com/problems/k-closest-points-to-origin
// Complexity: runtime O(nlogk), space O(logn) ignoring output, O(k) otherwise.

class Solution {
public:
    vector<vector<int>> kClosest(const vector<vector<int>> &points, const int K)
    {
        vector<vector<int>> res(K);
        partial_sort_copy(cbegin(points), cend(points), begin(res), end(res),
                          [](const auto &a, const auto &b) noexcept {
                              const auto da = a[0] * a[0] + a[1] * a[1];
                              const auto db = b[0] * b[0] + b[1] * b[1];
                              return da < db;
                          });
        return res;
    }
};
