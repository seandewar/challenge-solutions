// https://leetcode.com/problems/unique-paths/
//
// Creates a matrix of m*n entries, computing the number of different paths to
// each cell given that we may only move down or right, and returns the number
// of paths to the last cell (co-ords: m-1,n-1).
//
// Computing the number of paths to a cell involves simply adding the number of
// paths to the cell above and to the left of it; because we already know there
// will only be 1 possible path to 0,0 (our start) and no possible paths to any
// out-of-bounds cell outside the matrix, we can calculate the number of paths
// to any other cell from 0,0 to our target at m-1,n-1 in one pass of the m*n
// matrix.
//
// Complexity: runtime O(m*n), space O(m*n).

class Solution {
public:
    int uniquePaths(const int m, const int n) const
    {
        // NOTE: the unnecessary zeroing of grid's memory in this vector ctor
        // can be avoided by calling reserve(m * n) and using push_back()
        // instead for front() = 1 and grid[y * m + x] = left + top (if you care
        // enough...)
        vector<int> grid(m * n);
        grid.front() = 1; // 0,0 always has 1 path to itself

        for (int y = 0; y < n; ++y) {
            for (int x = 0; x < m; ++x) {
                if (x == 0 && y == 0) // always 1 path to 0,0; skip
                    continue;

                const int left = x > 0 ? grid[y * m + (x - 1)] : 0;
                const int top = y > 0 ? grid[(y - 1) * m + x] : 0;

                grid[y * m + x] = left + top;
            }
        }

        return grid.back(); // m-1,n-1
    }
};
