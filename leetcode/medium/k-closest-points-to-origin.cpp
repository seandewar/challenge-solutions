// https://leetcode.com/problems/k-closest-points-to-origin
//
// Complexity: runtime O(TODO), space O(TODO).

class Solution {
public:
    vector<vector<int>> kClosest(vector<vector<int>> &points, int K)
    {
        auto res = points;
        partial_sort(begin(res), end(res), begin(res),
                     [](const auto &a, const auto &b) noexcept {
                         const auto da = a[0] * a[0] + a[1] * a[1];
                         const auto db = b[0] * b[0] + b[1] * b[1];
                         return da < db;
                     });
        res.erase(begin(res) + K, end(res));
        return res;
    }
};
