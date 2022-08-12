// https://leetcode.com/problems/01-matrix
// Complexity: runtime O(m*n), space O(m*n).

class Solution {
public:
    vector<vector<int>> updateMatrix(vector<vector<int>> &mat) const
    {
        queue<pair<int, int>> q;
        for (int y = 0; y < size(mat); ++y) {
            for (int x = 0; x < size(mat[0]); ++x) {
                if (mat[y][x] == 0) {
                    q.emplace(x, y);
                    mat[y][x] = 1; // due to mat[y][x] <= d (otherwise < works)
                } else {
                    mat[y][x] = numeric_limits<int>::max();
                }
            }
        }

        for (int d = 0; !empty(q); ++d) {
            const auto sz = size(q);
            for (int i = 0; i < sz; ++i) {
                const auto [x, y] = q.front();
                q.pop();
                if (x < 0 || y < 0 || x >= size(mat[0]) || y >= size(mat)
                    || mat[y][x] <= d)
                    continue;

                mat[y][x] = d;
                q.emplace(x - 1, y);
                q.emplace(x + 1, y);
                q.emplace(x, y - 1);
                q.emplace(x, y + 1);
            }
        }
        return mat;
    }
};
