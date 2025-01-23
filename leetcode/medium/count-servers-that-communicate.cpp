// https://leetcode.com/problems/count-servers-that-communicate
// Complexity: runtime O(n*m), space O(n+m).

class Solution {
public:
    int countServers(const vector<vector<int>> &grid) const
    {
        const int n = grid.size(), m = grid[0].size();
        vector<int> row_counts(n), col_counts(m);
        for (int y = 0; y < n; ++y) {
            for (int x = 0; x < m; ++x) {
                if (grid[y][x] == 1) {
                    ++row_counts[y];
                    ++col_counts[x];
                }
            }
        }

        int ret = 0;
        for (int y = 0; y < n; ++y)
            for (int x = 0; x < m; ++x)
                if (grid[y][x] == 1 && (row_counts[y] > 1 || col_counts[x] > 1))
                    ++ret;

        return ret;
    }
};
