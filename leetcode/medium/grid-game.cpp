// https://leetcode.com/problems/grid-game
// This was fun!
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    long long gridGame(const vector<vector<int>> &grid) const
    {
        // Best strategy for robot 2 is either all numbers on the top row (plus
        // the rightmost on the bottom) or all on the bottom row (plus the
        // leftmost on the top). score_a walks down at the rightmost side of the
        // grid, and score_b does so at the leftmost.
        //
        // Assume the first robot starts with a down movement at the leftmost
        // side, then delaying it one move until we move down at the rightmost.
        auto score2_a =
                 accumulate(grid[0].begin() + 1, grid[0].end(), 0ll, plus{}),
             score2_b = 0ll;

        auto ret = score2_a;
        for (int j = 1; j < grid[0].size(); ++j) {
            score2_a -= grid[0][j];
            score2_b += grid[1][j - 1];
            ret = min(ret, max(score2_a, score2_b));
        }
        return ret;
    }
};
