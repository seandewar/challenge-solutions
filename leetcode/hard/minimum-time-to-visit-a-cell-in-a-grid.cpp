// https://leetcode.com/problems/minimum-time-to-visit-a-cell-in-a-grid
// Complexity: runtime O(nm*log(mn)), space O(mn).

class Solution {
public:
    int minimumTime(const vector<vector<int>> &grid) const
    {
        if (grid[0][1] > 1 && grid[1][0] > 1)
            return -1; // boxed-in with no wiggle room

        const int n = grid.size(), m = grid[0].size();
        array<bool, 100000> visited{true};
        struct Walker {
            int t, x, y;
        };
        priority_queue q(
            [](const Walker &a, const Walker &b) { return a.t > b.t; },
            vector{Walker{0, 0, 0}});
        while (true) {
            const auto top = q.top();
            q.pop();

            for (const auto [dx, dy] : {pair{1, 0}, {-1, 0}, {0, 1}, {0, -1}}) {
                const int x2 = top.x + dx, y2 = top.y + dy;
                if (x2 < 0 || y2 < 0 || x2 >= m || y2 >= n)
                    continue;
                if (exchange(visited[y2 * m + x2], true))
                    continue;

                int t2 = top.t + 1;
                if (grid[y2][x2] > t2)
                    t2 = grid[y2][x2] + (t2 + grid[y2][x2]) % 2; // wiggle
                if (x2 == m - 1 && y2 == n - 1)
                    return t2;

                q.emplace(t2, x2, y2);
            }
        }
    }
};
