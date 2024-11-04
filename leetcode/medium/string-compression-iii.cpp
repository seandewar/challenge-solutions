// https://leetcode.com/problems/string-compression-iii
// Complexity: runtime O(n), space O(n) [inc. output, O(1) otherwise].

class Solution {
public:
    string compressedString(const string &word) const
    {
        string comp;
        char pre_c = word.front();
        size_t pre_len = 1;
        for (size_t i = 1; i < word.size(); ++i) {
            if (word[i] != pre_c || pre_len == 9) {
                comp.push_back('0' + pre_len);
                comp.push_back(pre_c);
                pre_c = word[i];
                pre_len = 1;
                continue;
            }
            ++pre_len;
        }
        comp.push_back('0' + pre_len);
        comp.push_back(pre_c);
        return comp;
    }
};
