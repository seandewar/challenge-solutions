// https://leetcode.com/problems/cut-off-trees-for-golf-event
// BFS.
// Complexity: runtime O(m*n*trees) [O(m^2*n^2)], space O(m*n).

class Solution {
public:
    int cutOffTree(vector<vector<int>> &forest) const
    {
        if (forest[0][0] == 0) {
            return -1;
        }
        int w = size(forest[0]), h = size(forest);
        vector<pair<int, int>> trees;
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                if (forest[y][x] > 1) {
                    trees.emplace_back(x, y);
                }
            }
        }
        sort(begin(trees), end(trees), [&](auto &a, auto &b) {
            auto [x1, y1] = a;
            auto [x2, y2] = b;
            return forest[y1][x1] < forest[y2][x2];
        });
        deque<pair<int, int>> q;
        auto visit = [&](int x, int y) {
            if (x >= 0 && y >= 0 && x < w && y < h && forest[y][x] > 0) {
                forest[y][x] = -forest[y][x];
                q.emplace_back(x, y);
            }
        };
        visit(0, 0);
        for (int d = 0, i = 0; !empty(q); ++d) {
            for (int n = size(q); n > 0; --n) {
                auto [x, y] = q.front();
                q.pop_front();
                if (trees[i] == pair{x, y}) {
                    if (++i >= size(trees)) {
                        return d;
                    }
                    forest[y][x] = 1;
                    for (int y2 = 0; y2 < h; ++y2) {
                        for (int x2 = 0; x2 < w; ++x2) {
                            if (forest[y2][x2] < 0) {
                                forest[y2][x2] = -forest[y2][x2];
                            }
                        }
                    }
                    q.clear();
                    visit(x, y);
                    --d;
                    break;
                }
                visit(x + 1, y);
                visit(x - 1, y);
                visit(x, y + 1);
                visit(x, y - 1);
            }
        }
        return -1;
    }
};
