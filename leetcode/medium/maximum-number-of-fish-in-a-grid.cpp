// https://leetcode.com/problems/maximum-number-of-fish-in-a-grid
// Complexity: runtime O(n*m), space O(n*m).

class Solution {
public:
    int findMaxFish(vector<vector<int>> &grid) const
    {
        int ret = 0;
        for (int y = 0; y < grid.size(); ++y) {
            for (int x = 0; x < grid[0].size(); ++x) {
                if (grid[y][x] <= 0)
                    continue;

                const auto fish = [&](this const auto &self, const int x,
                                      const int y) {
                    if (x < 0 || y < 0 || x >= grid[0].size()
                        || y >= grid.size() || grid[y][x] <= 0)
                        return 0;

                    int sum = exchange(grid[y][x], 0);
                    for (const auto [dx, dy] :
                         {pair{1, 0}, {-1, 0}, {0, 1}, {0, -1}})
                        sum += self(x + dx, y + dy);

                    return sum;
                };

                ret = max(ret, fish(x, y));
            }
        }
        return ret;
    }
};
