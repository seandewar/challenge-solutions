// https://leetcode.com/problems/shift-2d-grid
//
// Complexity: runtime O(m*n), space O(1) [O(m*n) counting output].

class Solution {
public:
    vector<vector<int>> shiftGrid(const vector<vector<int>> &grid,
                                  const int k) const
    {
        const auto m = size(grid), n = size(grid[0]);
        vector<vector<int>> ret(m, vector<int>(n));
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                ret[(i + ((j + k) / n)) % m][(j + k) % n] = grid[i][j];
            }
        }

        return ret;
    }
};
