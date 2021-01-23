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
    template<bool Row0Only>
    void sortDiagonal(vector<vector<int>>& mat, const size_t j) const
    {
        const auto diagSize = Row0Only ? min(size(mat), size(mat[0]) - j)
                                       : min(size(mat) - j, size(mat[0]));
        vector<int> diag;
        diag.reserve(diagSize);
        for (size_t i = 0; i < diagSize; ++i)
            diag.push_back(Row0Only ? mat[i][j + i] : mat[j + i][i]);

        sort(begin(diag), end(diag));
        for (size_t i = 0; i < diagSize; ++i)
            (Row0Only ? mat[i][j + i] : mat[j + i][i]) = diag[i];
    }

public:
    vector<vector<int>> diagonalSort(vector<vector<int>>& mat) const
    {
        for (size_t y = 0; y < size(mat); ++y)
            sortDiagonal<false>(mat, y);
        for (size_t x = 1; x < size(mat[0]); ++x)
            sortDiagonal<true>(mat, x);

        return move(mat);
    }
};
