// https://leetcode.com/problems/pacific-atlantic-water-flow
//
// Iterative, space efficient solution.
//
// Sacrifices runtime performance for space.
// Space is cool.
//
// Complexity: space O(m*n).

class Solution {
public:
    vector<vector<int>> pacificAtlantic(vector<vector<int>>& matrix) const
    {
        const int h = size(matrix), w = empty(matrix) ? 0 : size(matrix[0]);
        vector visit(h, vector<pair<bool, bool>>(w));

        bool visChanged;
        const auto updateVisFromDir = [&](int x, int y, int nx, int ny) {
            auto &vis = visit[y][x], oldVis = vis;
            const bool canFlow = nx >= 0 && ny >= 0 && nx < w && ny < h
                                 && matrix[y][x] >= matrix[ny][nx];
            vis.first |= nx < 0 || ny < 0 || canFlow && visit[ny][nx].first;
            vis.second |= nx >= w || ny >= h || canFlow && visit[ny][nx].second;
            visChanged |= vis != oldVis;
        };

        do {
            visChanged = false;
            for (const auto [dx, dy] : {pair(0, 1), {1, 0}, {0, -1}, {-1, 0}})
                for (int y = 0; y < h; ++y)
                    for (int x = 0; x < w; ++x)
                        updateVisFromDir(x, y, x + dx, y + dy);
        } while (visChanged);

        vector<vector<int>> result;
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x)
                if (visit[y][x] == pair(true, true))
                    result.push_back(vector{y, x});

        return result;
    }
};

// Alternative Solution: Even more space efficient; better than 100% space
// solution.
//
// Abuse the std::vector<bool> specialization and reserve the capacity needed
// for our output. The use of the vector<bool> specialiazation alone can reduce
// the size of "visit" by up to 8 times! (on LC, where sizeof(pair<bool, bool>)
// == 2 && CHAR_BIT == 8)
//
// Complexity: space O(m*n).

class Solution {
public:
    vector<vector<int>> pacificAtlantic(vector<vector<int>>& matrix) const
    {
        const int h = size(matrix), w = empty(matrix) ? 0 : size(matrix[0]);
        vector<bool> visit(2 * w * h);
        const auto visGet = [&](int x, int y) -> pair<bool, bool> {
            return {visit[2 * (y * w + x)], visit[2 * (y * w + x) + 1]};
        };
        const auto visSet = [&](int x, int y, auto v) {
            visit[2 * (y * w + x)] = v.first;
            visit[2 * (y * w + x) + 1] = v.second;
        };

        bool visChanged;
        const auto updateVisFromDir = [&](int x, int y, int nx, int ny) {
            auto vis = visGet(x, y), oldVis = vis;
            const bool canFlow = nx >= 0 && ny >= 0 && nx < w && ny < h
                                 && matrix[y][x] >= matrix[ny][nx];
            vis.first |= nx < 0 || ny < 0 || canFlow && visGet(nx, ny).first;
            vis.second |= nx >= w || ny >= h || canFlow && visGet(nx, ny).second;
            visChanged |= vis != oldVis;
            visSet(x, y, vis);
        };

        do {
            visChanged = false;
            for (const auto [dx, dy] : {pair(0, 1), {1, 0}, {0, -1}, {-1, 0}})
                for (int y = 0; y < h; ++y)
                    for (int x = 0; x < w; ++x)
                        updateVisFromDir(x, y, x + dx, y + dy);
        } while (visChanged);

        vector<vector<int>> result;
        int numResults = 0;
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x)
                if (visGet(x, y) == pair(true, true))
                    ++numResults;

        result.reserve(numResults);
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x)
                if (visGet(x, y) == pair(true, true))
                    result.push_back(vector{y, x});

        return result;
    }
};
