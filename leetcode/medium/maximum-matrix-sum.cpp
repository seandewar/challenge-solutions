// https://leetcode.com/problems/maximum-matrix-sum
// Faster than 100%.
// Complexity: runtime O(n*m), space O(1).

class Solution {
public:
    long long maxMatrixSum(const vector<vector<int>> &matrix) const
    {
        long long sum = 0;
        int abs_min = numeric_limits<int>::max();
        bool even_neg_count = true;
        for (const auto &v : matrix) {
            for (const int x : v) {
                sum += abs(x);
                abs_min = min(abs_min, abs(x));
                if (x < 0)
                    even_neg_count = !even_neg_count;
            }
        }
        return even_neg_count ? sum : sum - 2 * abs_min;
    }
};
