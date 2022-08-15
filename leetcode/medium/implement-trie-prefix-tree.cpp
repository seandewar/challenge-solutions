// https://leetcode.com/problems/implement-trie-prefix-tree
// Complexity: runtime: all O(n), space O(n*w).

class Trie {
    struct Node {
        bool end_ = false;
        array<unique_ptr<Node>, 26> children_;
    };
    Node root_;

    Node *find(string_view word, bool insert = false)
    {
        auto node = &root_;
        for (auto c : word) {
            auto &next = node->children_[c - 'a'];
            if (!next) {
                if (insert) {
                    next = make_unique<Node>();
                } else {
                    return nullptr;
                }
            }
            node = next.get();
        }
        return node;
    }

public:
    Trie() = default;

    void insert(const string& word)
    {
        find(word, true)->end_ = true;
    }

    bool search(const string& word)
    {
        auto node = find(word);
        return node && node->end_;
    }

    bool startsWith(const string& prefix)
    {
        return find(prefix);
    }
};
