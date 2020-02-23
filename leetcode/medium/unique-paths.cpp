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

// Alternative Solution: mathematical solution using binomial coefficients
// (combinatorics "choose" function)
//
// This solution relies on the observation that, for example, given a 4*3
// matrix, a valid path from the top-left cell to the goal may look like this:
//
// [Down, Down, Down, Right, Right]
//
// ...or like this:
//
// [Down, Right, Down, Right, Down]
//
// You can see that each path takes exactly 3+2 ((m-1)+(n-1)) steps (the -1s are
// needed as we already start at the top-left square), with each path containing
// exactly 3 (m-1) down and 2 (n-1) right movements. You will also find that
// each unique path is a different combination of the steps in the other paths,
// therefore we can use the binomial coefficient formula to calculate the number
// of unique paths:
//
// C(n, r) = (n) choose (r) = n!/(r!*(n-r)!)
//
// Given a set of n elements, the formula results in the number of ways to
// choose r elements from the set (where order doesn't matter: choosing a,b is
// the same way as choosing b,a).
//
// For our example, there are C((4-1)+(3-1), 3-1) = C(5, 2) ways to go right in
// any path, with the rest of the movements being downward steps, or similarly,
// C(5, 3) ways to go down with the others being steps to the right (both
// produce the same result); therefore, there are 10 unique paths we can take to
// reach our goal in a 4*3 matrix.
//
// Therefore, how many unique paths we have in an m*n grid to the goal equals
// C((m-1)+(n-1), n-1) or C((m-1)+(n-1), m-1), which can be simplified as
// C(m+n-2, n-1) or C(m+n-2, m-1).
//
// Complexity: runtime O(m+n), space O(1).
/*
class Solution {
public:
    int uniquePaths(const int m, const int n) const
    {
        int result = 1;

        // as C(m+n-2,n-1)==C(m+n-2,m-1), we can select the smallest of m-1 &
        // n-1 as our loop count to minimise the number of iterations required
        for (int i = 1, chooseN = m + n - 2; i <= min(m, n) - 1; ++i) {

            // NOTE: result is cast to long long so that the variables in the
            // expression are promoted to long long, which avoids overflow from
            // the intermediate calculations. the implicit narrowing back to int
            // on the assignment to result is fine, as by then the value should
            // fit snugly in leetcode's int data type

            result = (static_cast<long long>(result) * chooseN--) / i;
        }

        return result;
    }
};
*/
