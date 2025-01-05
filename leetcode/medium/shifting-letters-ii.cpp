// https://leetcode.com/problems/shifting-letters-ii
// Complexity: runtime O(n+m), space O(n).

class Solution {
public:
    string shiftingLetters(string &s, const vector<vector<int>> &shifts) const
    {
        vector<int> shift_changes(s.size());
        for (const auto &sh : shifts) {
            const int shift = sh[2] ? 1 : -1;
            shift_changes[sh[0]] += shift;
            if (sh[1] + 1 < shift_changes.size())
                shift_changes[sh[1] + 1] -= shift;
        }

        for (int i = 0, shift = 0; i < s.size(); ++i) {
            shift += shift_changes[i];
            int letter_i = (s[i] - 'a' + shift) % 26;
            if (letter_i < 0)
                letter_i += 26;
            s[i] = letter_i + 'a';
        }
        return s;
    }
};
