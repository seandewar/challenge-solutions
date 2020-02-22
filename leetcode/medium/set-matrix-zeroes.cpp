// https://leetcode.com/problems/set-matrix-zeroes/
//
// This solution uses O(m+n) extra space, but a constant extra space solution
// exists (see the alternative solution below).
//
// See the code comments for details.
//
// Complexity: runtime O(m*n), space O(m+n).

class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) const
    {
        // a 0 in any cell will affect the whole row/column that cell is on, so
        // we need only keep track of which rows/columns contain a 0 in *any* of
        // their cells. therefore, we only need to allocate the space required
        // to store a single column and row in the matrix to do this: O(m+n).
        //
        // I refer to these as "flag tables", because I'm terrible at names
        vector colAnyZeroes(size(matrix), 1), rowAnyZeroes(size(matrix[0]), 1);

        // set the corresponding row/column in our "flag table" to 0 if any cell
        // in that row/column contains a 0
        for (vector<vector<int>>::size_type y = 0; y < size(matrix); ++y) {
            for (vector<int>::size_type x = 0; x < size(matrix[0]); ++x) {
                colAnyZeroes[y] = matrix[y][x] == 0 ? 0 : colAnyZeroes[y];
                rowAnyZeroes[x] = matrix[y][x] == 0 ? 0 : rowAnyZeroes[x];
            }
        }

        // for each 0 in our row/column "flag tables", set all the cells in the
        // entire corresponding row/column in the matrix to 0
        for (vector<vector<int>>::size_type y = 0; y < size(matrix); ++y) {
            for (vector<int>::size_type x = 0; x < size(matrix[0]); ++x) {
                if (colAnyZeroes[y] == 0 || rowAnyZeroes[x] == 0)
                    matrix[y][x] = 0;
            }
        }
    }
};

// Alternative Solution: Constant space solution.
//
// This solution works similarly to the one above, except we use the 1st row
// (top row) and column (left column) of the matrix itself as our "flag table".
//
// Complexity: runtime O(m*n), space O(1).
/*
class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) const
    {
        // because we will be using the 1st row & col of our matrix as the "flag
        // tables", we need to test if the original row & col contains any 0s
        // before we mess with it. if they do, the corresponding row/col will
        // need to be cleared after our subsequent looping and modification of
        // the rest of the matrix.
        //
        // if we don't do this, we wont know if the original 1st row/col
        // contained a 0, or if we wrote a 0 there as a flag in our subsequent
        // looping, so we wont know whether or not this row/col actually needed
        // clearing afterwards
        const bool zero1stRow = find(cbegin(*cbegin(matrix)),
                cend(*cbegin(matrix)), 0) != cend(*cbegin(matrix));

        const bool zero1stCol = find_if(cbegin(matrix), cend(matrix),
                [](const auto& row) { return row[0] == 0; }) != cend(matrix);

        // following 2 loops work on the same principles as the above solution,
        // except we start at x=1,y=1 for each loop so that we do not
        // erroneously clear parts of our "flag tables" to 0
        for (vector<vector<int>>::size_type y = 1; y < size(matrix); ++y) {
            for (vector<int>::size_type x = 1; x < size(matrix[0]); ++x) {
                matrix[y][0] = matrix[y][x] == 0 ? 0 : matrix[y][0];
                matrix[0][x] = matrix[y][x] == 0 ? 0 : matrix[0][x];
            }
        }

        for (vector<vector<int>>::size_type y = 1; y < size(matrix); ++y) {
            for (vector<int>::size_type x = 1; x < size(matrix[0]); ++x) {
                if (matrix[y][0] == 0 || matrix[0][x] == 0)
                    matrix[y][x] = 0;
            }
        }

        // now, if the row & col we used as "flag tables" originally contained
        // any 0s, we can safely clear the entire row/col now
        if (zero1stRow)
            fill(begin(*begin(matrix)), end(*begin(matrix)), 0);

        if (zero1stCol) {
            for (auto& row : matrix)
                row[0] = 0;
        }
    }
};
*/
