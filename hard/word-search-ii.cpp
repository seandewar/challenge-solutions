// https://leetcode.com/problems/word-search-ii/
//
// Uses a hash table for storing all prefixes. Prefer a trie-based approach.
//
// Complexity: runtime O(nw*(sw^2) + m*n*sw) [pre-process (+ hashing) + dfs],
//             space O(nw*sw) [pre-process].
// Where: m,n = board dimensions, nw = number of words, sw = size of a word

class Solution {
    using Board = vector<vector<char>>;
    using BoardWidth = Board::value_type::size_type;
    using BoardHeight = Board::size_type;

    // could use a trie, which will be faster due to no O(n) string hashing
    using PrefixMap = unordered_map<string_view, bool>;

    void dfs(vector<string>& results,
             Board& board,
             PrefixMap& prefixes,
             BoardWidth x,
             BoardHeight y,
             string word = "")
    {
        if (y >= size(board) || x >= size(board.front()) || board[y][x] == '\0')
            return;

        word += board[y][x];

        const auto fit = prefixes.find(word);
        if (fit == cend(prefixes))
            return;

        auto& [prefix, fullWord] = *fit;
        if (fullWord) {
            results.emplace_back(prefix.data(), size(prefix));
            fullWord = false; // stop duplicate results w/o needing to use a set
        }

        const auto cell = board[y][x];
        board[y][x] = '\0'; // mark as already visited

        dfs(results, board, prefixes, x + 1, y    , word);
        dfs(results, board, prefixes, x - 1, y    , word);
        dfs(results, board, prefixes, x    , y + 1, word);
        dfs(results, board, prefixes, x    , y - 1, word);

        board[y][x] = cell; // back-track: restore original cell
    }

public:
    vector<string> findWords(Board& board, vector<string>& words)
    {
        PrefixMap prefixes;

        for (const auto& word : words) {
            for (string::size_type i = 0; i < size(word); ++i) {
                const string_view prefix(word.data(), i + 1);
                const bool fullWord = size(prefix) == size(word);

                const auto fit = prefixes.find(prefix);
                if (fit == cend(prefixes))
                    prefixes.emplace(prefix, fullWord);
                else
                    fit->second = fit->second || fullWord;
            }
        }

        vector<string> results;

        for (BoardHeight y = 0; y < size(board); ++y)
            for (BoardWidth x = 0; x < size(board.front()); ++x)
                dfs(results, board, prefixes, x, y);

        return results;
    }
};
