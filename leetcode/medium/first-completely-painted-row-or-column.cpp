// https://leetcode.com/problems/first-completely-painted-row-or-column
// Complexity: runtime O(n*m), space O(n*m).

class Solution {
public:
    int firstCompleteIndex(const vector<int> &arr,
                           const vector<vector<int>> &mat) const
    {
        const int n = mat.size(), m = mat[0].size();
        vector positions(arr.size(), pair{-1, -1});
        vector rows_rem(n, m), cols_rem(m, n);
        for (int y = 0; y < n; ++y)
            for (int x = 0; x < m; ++x)
                positions[mat[y][x] - 1] = {x, y};

        for (int i = 0; i < arr.size(); ++i) {
            const auto [x, y] = positions[arr[i] - 1];
            if (--rows_rem[y] == 0 || --cols_rem[x] == 0)
                return i;
        }

        abort(); // should be impossible
    }
};
