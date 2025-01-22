// https://leetcode.com/problems/map-of-highest-peak
// Complexity: runtime O(n*m), space O(n*m) [reuses input storage for output,
// but the space complexity is bounded by the BFS queue].

class Solution {
public:
    vector<vector<int>> highestPeak(vector<vector<int>> &grid) const
    {
        constexpr array<pair<int, int>, 4> dirs{
            {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};

        const int grid_h = grid.size(), grid_w = grid[0].size();
        const auto in_bounds = [&](const int x, const int y) {
            return x >= 0 && y >= 0 && x < grid_w && y < grid_h;
        };

        deque<pair<int, int>> q;
        for (int y = 0; y < grid_h; ++y) {
            for (int x = 0; x < grid_w; ++x) {
                if (grid[y][x] != 1)
                    continue;

                for (const auto [dx, dy] : dirs) {
                    const int x2 = x + dx, y2 = y + dy;
                    if (!in_bounds(x2, y2) || grid[y2][x2] != 0)
                        continue;

                    grid[y2][x2] = numeric_limits<int>::max();
                    q.emplace_back(x2, y2);
                }
            }
        }

        for (auto &row : grid)
            for (int &cell : row)
                cell = cell == 1 ? 0 : numeric_limits<int>::max();

        for (const auto [x, y] : q)
            grid[y][x] = 1;

        for (int h = 1; !q.empty(); ++h) {
            for (auto n = q.size(); n-- > 0;) {
                const auto [x, y] = q.front();
                q.pop_front();

                for (const auto [dx, dy] : dirs) {
                    const int x2 = x + dx, y2 = y + dy;
                    if (!in_bounds(x2, y2) || grid[y2][x2] <= h + 1)
                        continue;

                    grid[y2][x2] = h + 1;
                    q.emplace_back(x2, y2);
                }
            }
        }
        return grid;
    }
};
