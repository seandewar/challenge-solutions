// https://leetcode.com/problems/minimum-obstacle-removal-to-reach-corner
//
// Faster than 100%. The basic idea is that we fill empty space from the start
// position, then allow ourselves to remove adjacent walls once per iteration;
// the minimum amount of walls to remove will be the number of iterations. The
// order in which we process the queued squares doesn't really matter, so only a
// (FILO) vector is used, as it's most efficient.
//
// Complexity: runtime O(m*n), space O(m+n) [input is reused for marking visits;
// the queues only ever hold the outer edges of the filled area, which is m+n in
// the worse case].

class Solution {
public:
    int minimumObstacles(vector<vector<int>> &grid) const
    {
        grid[0][0] = -1; // mark as visited
        vector<pair<int, int>> todo{{{0, 0}}}, todo_next;
        for (int ret = 0;; ++ret) {
            while (!todo.empty()) {
                const auto [x, y] = todo.back();
                todo.pop_back();
                for (const auto [dx, dy] :
                     {pair{1, 0}, {-1, 0}, {0, 1}, {0, -1}}) {
                    const int x2 = x + dx, y2 = y + dy;
                    if (x2 < 0 || y2 < 0 || x2 >= grid[0].size()
                        || y2 >= grid.size())
                        continue;
                    if (x2 == grid[0].size() - 1 && y2 == grid.size() - 1)
                        return ret;

                    if (grid[y2][x2] == 0)
                        todo.emplace_back(x2, y2);
                    else if (grid[y2][x2] == 1)
                        todo_next.emplace_back(x2, y2);
                    grid[y2][x2] = -1; // mark as visited
                }
            }
            todo = todo_next;
            todo_next.clear();
        }
    }
};
