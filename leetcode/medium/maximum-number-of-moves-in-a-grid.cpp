// https://leetcode.com/problems/maximum-number-of-moves-in-a-grid
// Complexity: runtime O(n*m), space O(n*m).

class Solution {
public:
    int maxMoves(const vector<vector<int>> &grid) const
    {
        vector<vector<int>> mem(
            grid.size(), vector(grid[0].size(), numeric_limits<int>::min()));
        int ret = 0;
        for (int x = 0; x < grid[0].size(); ++x) {
            for (int y = 0; y < grid.size(); ++y) {
                if (x == 0) {
                    mem[y][x] = 0;
                    continue;
                }
                for (int y2 = max(0, y - 1); y2 < min<int>(grid.size(), y + 2);
                     ++y2) {
                    if (grid[y2][x - 1] < grid[y][x])
                        mem[y][x] = max(mem[y][x], mem[y2][x - 1] + 1);
                }
                ret = max(ret, mem[y][x]);
            }
        }
        return ret;
    }
};
