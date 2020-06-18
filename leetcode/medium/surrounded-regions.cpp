// https://leetcode.com/problems/surrounded-regions/
//
// Read code comments for more details.
// Complexity: runtime O(m*n), space O(m*n) [recursion stack].

class Solution {
    void dfsMark(vector<vector<char>>& board,
                 const size_t x, const size_t y) const
    {
        if (y >= size(board) || x >= size(board[0]) || board[y][x] != 'O')
            return;

        // mark for preservation by editing this cell's value from O to o. we
        // will restore the original value before solve() returns
        board[y][x] = 'o';

        dfsMark(board, x + 1, y    );
        dfsMark(board, x - 1, y    );
        dfsMark(board, x    , y + 1);
        dfsMark(board, x    , y - 1);
    }

public:
    void solve(vector<vector<char>>& board) const
    {
        if (empty(board))
            return;

        // O regions that connect to the edge of the board cannot be fully
        // surrounded by Xs, so we only need to check the edges for such regions
        // and mark them so that we can preserve them. we mark the regions to be
        // preserved by editing their value from O to o
        for (size_t y = 0; y < size(board); ++y) {
            dfsMark(board, 0, y);
            dfsMark(board, size(board[0]) - 1, y);
        }

        for (size_t x = 0; x < size(board[0]); ++x) {
            dfsMark(board, x, 0);
            dfsMark(board, x, size(board) - 1);
        }

        // replace all unmarked O regions with Xs, and then restore the marked
        // regions to their original O values
        for (auto& r : board) {
            replace(begin(r), end(r), 'O', 'X');
            replace(begin(r), end(r), 'o', 'O');
        }
    }
};
