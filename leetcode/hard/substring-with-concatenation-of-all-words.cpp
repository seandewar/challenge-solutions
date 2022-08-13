// https://leetcode.com/problems/substring-with-concatenation-of-all-words
// Complexity: runtime O(s*w*words), space O(w*words).

class Solution {
public:
    vector<int> findSubstring(const string s, const vector<string> &words) const
    {
        struct TrieNode {
            array<unique_ptr<TrieNode>, 26> next;
            int wcounti = -1;
        };
        vector<int> wcounts;
        TrieNode root;
        for (const auto &w : words) {
            auto node = &root;
            for (const char c : w) {
                auto &next = node->next[c - 'a'];
                if (!next)
                    next = make_unique<TrieNode>();
                node = next.get();
            }
            if (node->wcounti == -1) {
                node->wcounti = size(wcounts);
                wcounts.push_back(1);
            } else {
                ++wcounts[node->wcounti];
            }
        }

        const auto wsize = size(words[0]);
        vector<int> rem_wcounts, ret;
        for (int i = 0; i + size(words) * wsize - 1 < size(s); ++i) {
            rem_wcounts = wcounts;
            int j = 0;
            for (; j < size(words); ++j) {
                const auto w = string_view(s).substr(i + j * wsize, wsize);
                auto node = &root;
                for (const char c : w) {
                    if (!(node = node->next[c - 'a'].get()))
                        break;
                }
                if (!node || rem_wcounts[node->wcounti] == 0)
                    break;
                --rem_wcounts[node->wcounti];
            }
            if (j == size(words))
                ret.push_back(i);
        }
        return ret;
    }
};
