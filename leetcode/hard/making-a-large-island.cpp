// https://leetcode.com/problems/making-a-large-island
//
// Code's a little messy because this is raw and unedited straight from a time
// challenge.

class Solution {
    vector<int> areas;

    void calcArea(vector<vector<int>> &grid, int x, int y, int i = -1)
    {
        if (x < 0 || y < 0 || y >= grid.size() || x >= grid[0].size()
            || grid[y][x] <= 0) {
            return;
        }

        if (i == -1) {
            areas.push_back(1);
            i = areas.size();
        } else {
            ++areas[i - 1];
        }
        grid[y][x] = -i;

        calcArea(grid, x + 1, y, i);
        calcArea(grid, x - 1, y, i);
        calcArea(grid, x, y + 1, i);
        calcArea(grid, x, y - 1, i);
    }

public:
    int largestIsland(vector<vector<int>> &grid)
    {
        for (int y = 0; y < grid.size(); ++y) {
            for (int x = 0; x < grid[0].size(); ++x) {
                calcArea(grid, x, y);
            }
        }

        auto maxit = max_element(begin(areas), end(areas));
        int maxa = maxit == end(areas) ? 0 : *maxit;
        for (int y = 0; y < grid.size(); ++y) {
            for (int x = 0; x < grid[0].size(); ++x) {
                if (grid[y][x] == 0) {
                    array<int, 4> idxs;
                    idxs[0] =
                        x > 0 && grid[y][x - 1] < 0 ? -grid[y][x - 1] : -1;
                    idxs[1] = y < grid.size() - 1 && grid[y + 1][x] < 0
                                  ? -grid[y + 1][x]
                                  : -1;
                    idxs[2] = x < grid[0].size() - 1 && grid[y][x + 1] < 0
                                  ? -grid[y][x + 1]
                                  : -1;
                    idxs[3] =
                        y > 0 && grid[y - 1][x] < 0 ? -grid[y - 1][x] : -1;

                    int sum = 1;
                    for (int i = 0; i < idxs.size(); ++i) {
                        if (idxs[i] > 0) {
                            if (any_of(begin(idxs), begin(idxs) + i,
                                       [&](int v) { return v == idxs[i]; })) {
                                continue;
                            }
                            sum += areas[idxs[i] - 1];
                        }
                    }

                    maxa = max(maxa, sum);
                }
            }
        }

        return maxa;
    }
};
