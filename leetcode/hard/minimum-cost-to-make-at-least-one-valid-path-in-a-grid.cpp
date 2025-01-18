// https://leetcode.com/problems/minimum-cost-to-make-at-least-one-valid-path-in-a-grid
// Faster than 98%.
// Complexity: runtime O(n*m), space O(n*m).

class Solution {
public:
    int minCost(vector<vector<int>> &grid) const
    {
        constexpr array<pair<int, int>, 4> dirs{
            {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};
        constexpr int visited = 0;

        const int width = grid[0].size(), height = grid.size();
        if (width == 1 && height == 1)
            return 0;

        struct Walker {
            int x, y;
            int dir_i;
        };
        deque<Walker> fill_queue{{0, 0, grid[0][0] - 1}};
        grid[0][0] = visited;

        const auto visit_found_end = [&](const int x, const int y) {
            if (x < 0 || y < 0 || x >= width || y >= height)
                return false;
            if (x == width - 1 && y == height - 1)
                return true;
            if (grid[y][x] == visited)
                return false;

            fill_queue.emplace_back(x, y, grid[y][x] - 1);
            grid[y][x] = visited;
            return false;
        };

        for (int cost = 0; !fill_queue.empty();) {
            int i = 0;
            for (; i < fill_queue.size(); ++i) {
                const auto &walker = fill_queue[i];
                const auto [dx, dy] = dirs[walker.dir_i];
                if (visit_found_end(walker.x + dx, walker.y + dy))
                    return cost;
            }

            ++cost;
            for (int j = 0; j < i; ++j) {
                const auto &walker = fill_queue.front();

                for (int dir_i = 0; dir_i < dirs.size(); ++dir_i) {
                    if (dir_i == walker.dir_i)
                        continue;

                    const auto [dx, dy] = dirs[dir_i];
                    if (visit_found_end(walker.x + dx, walker.y + dy))
                        return cost;
                }

                fill_queue.pop_front();
            }
        }

        abort(); // end should be reachable
    }
};
