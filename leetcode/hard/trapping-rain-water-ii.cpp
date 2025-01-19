// https://leetcode.com/problems/trapping-rain-water-ii
//
// Faster is possible (possibly using a heap), but this passes.
//
// Complexity: runtime O(n^2*m^2), space O(n*m).

class Solution {
public:
    int trapRainWater(vector<vector<int>> &heights) const
    {
        constexpr array<pair<int, int>, 4> dirs{
            {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};

        constexpr unsigned no_puddle_bit = 1 << (CHAR_BIT * sizeof(int) - 1),
                           filled_bit = no_puddle_bit >> 1;
        constexpr int height_mask = ~(no_puddle_bit | filled_bit);

        const int n = heights.size(), m = heights[0].size();
        int ret = 0;
        vector<pair<int, int>> fills;
        for (int y0 = 1; y0 < n - 1; ++y0) {
            for (int x0 = 1; x0 < m - 1; ++x0) {
                int &v = heights[y0][x0];
                if (v & no_puddle_bit)
                    continue;

                fills = {{x0, y0}};
                v |= filled_bit;

                const int h = v & height_mask;
                int puddle_h = height_mask;
                for (int i = 0; i < fills.size(); ++i) {
                    const auto [x, y] = fills[i];

                    for (const auto [dx, dy] : dirs) {
                        const int x2 = x + dx, y2 = y + dy;
                        if (x2 < 0 || y2 < 0 || x2 >= m || y2 >= n) {
                            // rain flows out-of-bounds
                            puddle_h = no_puddle_bit;
                            continue;
                        }

                        int &v2 = heights[y2][x2];
                        if (v2 & filled_bit)
                            continue;

                        const int h2 = v2 & height_mask;
                        if (h2 <= h && (v2 & no_puddle_bit)) {
                            // rain eventually flows out-of-bounds
                            puddle_h = no_puddle_bit;
                            continue;
                        }
                        if (h2 > h) {
                            if (!(puddle_h & no_puddle_bit))
                                puddle_h = min(puddle_h, h2);
                            continue;
                        }

                        fills.emplace_back(x2, y2);
                        v2 |= filled_bit;
                    }
                }

                if (puddle_h & no_puddle_bit) {
                    // Mark only cells with height h as not having a puddle, as
                    // cells at lower heights may still contain puddles.
                    for (const auto [x2, y2] : fills) {
                        int &v2 = heights[y2][x2];
                        v2 &= ~filled_bit;

                        if ((v2 & height_mask) == h)
                            v2 |= no_puddle_bit;
                    }
                    continue;
                }

                for (const auto [x2, y2] : fills) {
                    int &v2 = heights[y2][x2];
                    ret += puddle_h - (v2 & height_mask);
                    v2 = puddle_h;
                }
            }
        }
        return ret;
    }
};
