// https://leetcode.com/problems/word-search/
//
// Complexity: runtime O(m*n*3^sw) [dfs (3-way after 2nd call due to visited)],
//             space O(sw) [recursion overhead].
// Where: m,n = board dimensions, sw = size of word

class Solution {
    using Board = vector<vector<char>>;

    bool tryFindWord(Board& board,
                     const string_view remWord,
                     const size_t x,
                     const size_t y) const
    {
        // -ve indices will underflow, so unless our board is bonkers huge, they
        // will still be caught by this condition
        if (y >= size(board) || x >= size(board[0])
            || board[y][x] != remWord[0])
            return false;

        if (size(remWord) == 1 && board[y][x] == remWord[0])
            return true;

        const char original = exchange(board[y][x], '\0'); // mark as visited
        const bool isFound = tryFindWord(board, remWord.substr(1), x + 1, y)
                             || tryFindWord(board, remWord.substr(1), x - 1, y)
                             || tryFindWord(board, remWord.substr(1), x, y + 1)
                             || tryFindWord(board, remWord.substr(1), x, y - 1);

        board[y][x] = original; // remove mark and restore original value
        return isFound;
    }

public:
    bool exist(Board& board, const string& word) const
    {
        // input constraints don't allow for empty word, so no need to check
        for (size_t y = 0; y < size(board); ++y) {
            for (size_t x = 0; x < size(board[0]); ++x) {
                if (board[y][x] == word[0] && tryFindWord(board, word, x, y))
                    return true;
            }
        }

        return false;
    }
};
