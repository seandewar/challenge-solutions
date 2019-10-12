// https://leetcode.com/problems/number-of-islands/
//
// This approach modifies the input array.
// Complexity: runtime O(m*n), space O(m*n).

class Solution {
    void fillIsland(vector<vector<char>>& grid, int x, int y)
    {
        if (x < 0 || x >= size(grid[0])
            || y < 0 || y >= size(grid)
            || grid[y][x] == '0') {
            return;
        }

        grid[y][x] = '0';

        fillIsland(grid, x + 1, y    );
        fillIsland(grid, x - 1, y    );
        fillIsland(grid, x    , y + 1);
        fillIsland(grid, x    , y - 1);
    }

public:
    int numIslands(vector<vector<char>>& grid)
    {
        int islands = 0;

        for (int y = 0; y < size(grid); ++y) {
            for (int x = 0; x < size(grid[0]); ++x) {
                if (grid[y][x] == '1') {
                    ++islands;
                    fillIsland(grid, x, y);
                }
            }
        }

        return islands;
    }
};
