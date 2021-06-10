// https://leetcode.com/problems/sort-the-matrix-diagonally
//
// Uses an auxiliary array for sorting.
//
// Sorting can be done in-place, but to do so without implementing your own
// sorting algorithm, you will have to create your own random-access iterator
// type for std::sort().
//
// Complexity: runtime O(m*n*log(min(m,n)), space O(max(m,n)).

class Solution {
public:
    vector<vector<int>> diagonalSort(vector<vector<int>>& mat) const
    {
        const auto m = size(mat), n = size(mat[0]);
        vector<int> diag;
        diag.reserve(min(m, n));

        for (size_t i = 0; i < n + m - 1; ++i) {
            ptrdiff_t x = min(n - 1, i), y = min(m, n + m - i) - 1;
            while (y >= 0 && x >= 0)
                diag.push_back(mat[y--][x--]);

            sort(begin(diag), end(diag));
            for (int v : diag)
                mat[++y][++x] = v;

            diag.clear();
        }

        return mat;
    }
};
