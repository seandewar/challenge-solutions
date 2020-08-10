// https://leetcode.com/problems/rotting-oranges/
//
// Complexity: runtime O(m*n), space O(1).

class Solution {
    bool tryMarkAsRotting(vector<vector<int>>& grid, const size_t x,
                          const size_t y) const noexcept
    {
        if (y < size(grid) && x < size(grid[0]) && grid[y][x] == 1) {
            grid[y][x] = -1;
            return true;
        }

        return false;
    }

public:
    int orangesRotting(vector<vector<int>>& grid) const noexcept
    {
        int result = 0;

        while (true) {
            bool newRots = false;

            for (size_t y = 0; y < size(grid); ++y) {
                for (size_t x = 0; x < size(grid[0]); ++x) {
                    if (grid[y][x] == 2) {
                        // mark the adjacent oranges as to be rotten for later
                        newRots |= tryMarkAsRotting(grid, x + 1, y);
                        newRots |= tryMarkAsRotting(grid, x - 1, y);
                        newRots |= tryMarkAsRotting(grid, x, y + 1);
                        newRots |= tryMarkAsRotting(grid, x, y - 1);
                    }
                }
            }

            if (!newRots) {
                break;
            }

            // rot the marked oranges
            for (auto& row : grid) {
                replace(begin(row), end(row), -1, 2);
            }

            ++result;
        }

        if (any_of(cbegin(grid), cend(grid), [](const auto& row) {
                return find(cbegin(row), cend(row), 1) != cend(row);
            })) {
            return -1;
        }

        return result;
    }
};
