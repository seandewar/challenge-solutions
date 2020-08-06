// https://leetcode.com/problems/add-and-search-word-data-structure-design/
//
// Trie + dfs for wildcard characters.
//
// Complexity: addWord() runtime O(m), search() runtime O(n), space O(n) [where
// n = num chars in all words, m = num chars in a single input word].

class WordDictionary {
    struct Node {
        bool completesWord = false;
        array<unique_ptr<Node>, 26> children{};
    } root;

    bool search(const string_view word, const Node* root) const
    {
        for (size_t i = 0; i < size(word); ++i) {
            if (word[i] == '.') {
                const auto pred = [&](const unique_ptr<Node>& child) {
                    return child
                           && search({&word[i + 1], size(word) - i - 1},
                                     child.get());
                };

                return any_of(cbegin(root->children), cend(root->children),
                              pred);
            }

            if (!(root = root->children[word[i] - 'a'].get())) {
                return false;
            }
        }

        return root->completesWord;
    }

public:
    /** Initialize your data structure here. */
    WordDictionary() = default;

    /** Adds a word into the data structure. */
    void addWord(const string_view word)
    {
        auto n = &root;

        for (const char c : word) {
            const size_t i = c - 'a';

            if (!n->children[i]) {
                n->children[i].reset(new Node);
            }

            n = n->children[i].get();
        }

        n->completesWord = true;
    }

    /** Returns if the word is in the data structure. A word could contain the
     * dot character '.' to represent any one letter. */
    bool search(const string_view word) const { return search(word, &root); }
};

/**
 * Your WordDictionary object will be instantiated and called as such:
 * WordDictionary* obj = new WordDictionary();
 * obj->addWord(word);
 * bool param_2 = obj->search(word);
 */
