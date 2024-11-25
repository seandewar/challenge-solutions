// https://leetcode.com/problems/sliding-puzzle
//
// Breadth-first runtime would be much better, but depth-first (with pruning)
// was faster to write, and passes.
//
// Complexity: runtime O((n*m)*(n*m)!), space O((n*m)!), I think.

class Solution {
    static constexpr uint32_t goal_memo_key =
        (5 << 3) | (4 << 6) | (3 << 9) | (2 << 12) | (1 << 15);

    void dfs(vector<vector<int>> &board, unordered_map<uint32_t, int> &memo,
             const int len = 0) const
    {
        uint32_t memo_key = 0;
        for (const auto &row : board)
            for (const unsigned x : row)
                memo_key = (memo_key << 3) | x;

        if (const auto it = memo.find(memo_key);
            it != memo.end() && it->second <= len)
            return;

        memo[memo_key] = len;
        if (memo_key == goal_memo_key)
            return;

        for (int y = 0; y < board.size(); ++y) {
            for (int x = 0; x < board[0].size(); ++x) {
                if (board[y][x] != 0)
                    continue;

                for (const auto [dx, dy] :
                     {pair{-1, 0}, {1, 0}, {0, -1}, {0, 1}}) {
                    const int x2 = x + dx, y2 = y + dy;
                    if (x2 < 0 || y2 < 0 || x2 >= board[0].size()
                        || y2 >= board.size())
                        continue;

                    swap(board[y2][x2], board[y][x]);
                    dfs(board, memo, len + 1);
                    swap(board[y2][x2], board[y][x]); // back-track
                }
                break;
            }
        }
    }

public:
    int slidingPuzzle(vector<vector<int>> &board) const
    {
        unordered_map<uint32_t, int> memo;
        dfs(board, memo);
        const auto it = memo.find(goal_memo_key);
        return it != memo.end() ? it->second : -1;
    }
};
