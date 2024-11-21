// https://leetcode.com/problems/count-unguarded-cells-in-the-grid
// Faster than 100%.
// Complexity: runtime O(m*n), space O(m*n).

class Solution {
public:
    int countUnguarded(const int m, const int n,
                       const vector<vector<int>> &guards,
                       const vector<vector<int>> &walls) const
    {
        array<unsigned char, 100000> grid{};
        const auto idx = [=](const int x, const int y) { return y * n + x; };
        for (const auto &v : walls)
            grid[idx(v[1], v[0])] = 0b1111;

        const auto guard = [&](int x, int y, const int dx, const int dy,
                               const unsigned char dir_mask) {
            for (; x >= 0 && y >= 0 && x < n && y < m; x += dx, y += dy) {
                if (grid[idx(x, y)] & dir_mask)
                    break;
                grid[idx(x, y)] |= dir_mask;
            }
        };
        for (const auto &v : guards) {
            guard(v[1], v[0], 1, 0, 0b1000);
            guard(v[1], v[0], -1, 0, 0b0100);
            guard(v[1], v[0], 0, 1, 0b0010);
            guard(v[1], v[0], 0, -1, 0b0001);
        }

        return count(grid.begin(), grid.begin() + idx(n, m - 1), 0b0000);
    }
};
