// https://leetcode.com/problems/word-search-ii/
//
// Uses a trie to store the prefixes for all words. Performs a DFS on all board
// cells to try and match visited cells to the full words, keeping track of
// visited cells as necessary (and back-tracks once a particular DFS invocation
// is finished).
//
// Complexity: runtime O(m*n*3^sw) [dfs (3-way after 2nd call due to visited)],
//             space O(nw*sw) [pre-process].
// Where: m,n = board dimensions, nw = number of words, sw = size of a word

class Solution {
    using Board = vector<vector<char>>;
    using BoardWidth = Board::value_type::size_type;
    using BoardHeight = Board::size_type;

    struct TrieNode {
        unordered_map<char, unique_ptr<TrieNode>> children;
        string_view fullWord;

        void insertWord(string_view word)
        {
            auto node = this;

            for (const char c : word) {
                const auto fit = node->children.find(c);
                if (fit != cend(node->children))
                    node = fit->second.get();
                else
                    node = (node->children[c] = make_unique<TrieNode>()).get();
            }

            node->fullWord = word;
        }
    };

    void dfs(Board& board, BoardWidth x, BoardHeight y, TrieNode& root,
             vector<string>& results)
    {
        if (y >= size(board) || x >= size(board.front()) || !board[y][x])
            return;

        const auto fit = root.children.find(board[y][x]);
        if (fit == cend(root.children))
            return;

        auto& next = *fit->second;

        if (!empty(next.fullWord)) {
            results.emplace_back(next.fullWord.data(), size(next.fullWord));
            next.fullWord = string_view(); // avoids duplicate results
        }

        const char original = board[y][x];
        board[y][x] = '\0'; // mark as visited for this invocation

        dfs(board, x + 1, y    , next, results);
        dfs(board, x - 1, y    , next, results);
        dfs(board, x    , y + 1, next, results);
        dfs(board, x    , y - 1, next, results);

        board[y][x] = original; // back-track: restore original cell
    }

public:
    vector<string> findWords(Board& board, vector<string>& words)
    {
        TrieNode root;
        for (const auto& word : words)
            root.insertWord(word);

        vector<string> results;
        for (BoardHeight y = 0; y < size(board); ++y)
            for (BoardWidth x = 0; x < size(board.front()); ++x)
                dfs(board, x, y, root, results);

        return results;
    }
};

// Alternative Solution:
// Uses a hash table for storing all prefixes. Prefer a trie-based approach.
//
// Complexity: runtime O(m*n*3^sw) [dfs (3-way after 2nd call due to visited)],
//             space O(nw*sw) [pre-process].
// Where: m,n = board dimensions, nw = number of words, sw = size of a word
//
// Note that pre-process runtime is O(nw*sw^2) due to O(sw) hashing + O(sw)
// prefix looping & O(nw) word looping. While not as significant as the dfs
// complexity, it's still decently significant.

/*
class Solution {
    using Board = vector<vector<char>>;
    using BoardWidth = Board::value_type::size_type;
    using BoardHeight = Board::size_type;

    // could use a trie, which will be faster due to no O(n) string hashing
    using PrefixMap = unordered_map<string_view, bool>;

    void dfs(vector<string>& results, Board& board, PrefixMap& prefixes,
             BoardWidth x, BoardHeight y, string word = "")
    {
        if (y >= size(board) || x >= size(board.front()) || !board[y][x])
            return;

        word += board[y][x];

        if (const auto fit = prefixes.find(word); fit == cend(prefixes))
            return;
        else if (auto& [prefix, fullWord] = *fit; fullWord) {
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
*/
