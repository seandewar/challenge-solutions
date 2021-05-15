// https://leetcode.com/problems/valid-number
//
// Regex.
// Complexity: runtime likely O(n).

class Solution {
public:
    bool isNumber(const string s) const
    {
        static const regex r(R"_([+-]?(\d+(\.\d*)?|\.\d+)([eE][+-]?\d+)?)_");
        return regex_match(s, r);
    }
};
