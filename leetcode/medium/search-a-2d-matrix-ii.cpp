// https://leetcode.com/problems/search-a-2d-matrix-ii
//
// Not the most efficient approach, but still efficient enough to pass.
// See code comments for details.
//
// Complexity: runtime O(logh + h*logw), space O(h) [for col, which isn't
// strictly necessary, but allows us to easily use std::lower_bound()].

class Solution {
public:
    bool searchMatrix(const vector<vector<int>>& matrix, const int target) const
    {
        const auto height = size(matrix), width = size(matrix[0]);

        // search for the target in the first row
        const auto& row = matrix[0];
        const auto rowIt = lower_bound(cbegin(row), cend(row), target);
        if (rowIt != cend(row) && *rowIt == target)
            return true;

        const size_t x = rowIt - cbegin(row) - 1;
        if (x >= width)
            return false;

        // we couldn't find the target in the first row, so search the column
        // starting at the last value that was lower than our target in the
        // first row; we'll create a vector of the column contents so we can use
        // lower_bound() without doing lots of iterator stuff (ranges would be
        // lovely here)
        vector<int> col;
        col.reserve(height);
        for (const auto& row : matrix)
            col.push_back(row[x]);

        const auto colIt = lower_bound(cbegin(col), cend(col), target);
        if (colIt != cend(col) && *colIt == target)
            return true;

        // couldn't find the target in the column, so search the row starting at
        // the column with the last value lower than our target. if we still
        // can't find the target, search the rows after that row. if the first
        // value in the row is larger than our target, then there are no more
        // rows that could possibly have our target, and we can terminate early
        for (size_t y = colIt - cbegin(col) - 1;
             y < height && matrix[y][0] <= target; ++y) {
            if (binary_search(cbegin(matrix[y]), cbegin(matrix[y]) + x, target))
                return true;
        }

        return false;
    }
};
