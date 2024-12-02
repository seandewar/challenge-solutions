// https://leetcode.com/problems/check-if-a-word-occurs-as-a-prefix-of-any-word-in-a-sentence
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int isPrefixOfWord(const string &sentence, const string &word) const
    {
        for (int i = 0, word_i = 1; i < sentence.size(); ++word_i) {
            const int j = min(sentence.size(), sentence.find_first_of(' ', i));
            const string_view cand{sentence.begin() + i, sentence.begin() + j};
            if (cand.substr(0, word.size()) == word)
                return word_i;
            i = j + 1;
        }
        return -1;
    }
};
