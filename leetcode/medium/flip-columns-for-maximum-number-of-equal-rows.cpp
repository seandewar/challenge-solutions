// https://leetcode.com/problems/flip-columns-for-maximum-number-of-equal-rows
// Complexity: runtime O(n*m), space O(n*m).

class Solution {
public:
    int maxEqualRowsAfterFlips(const vector<vector<int>> &matrix) const
    {
        unordered_map<vector<bool>, int> row_freqs;
        vector<bool> key;
        key.reserve(matrix[0].size());
        int ret = 0;
        for (const auto &row : matrix) {
            key.clear();
            copy(row.begin(), row.end(), back_inserter(key));
            ret = max(ret, ++row_freqs[key]);
            transform(key.begin(), key.end(), key.begin(), logical_not{});
            ++row_freqs[key];
        }
        return ret;
    }
};
