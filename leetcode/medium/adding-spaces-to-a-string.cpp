// https://leetcode.com/problems/adding-spaces-to-a-string
// No need to sort "spaces", as it's already sorted via the constraints.
// Complexity: runtime O(n+s), space O(n+s) [including output; O(1) otherwise].

class Solution {
public:
    string addSpaces(const string &s, const vector<int> &spaces) const
    {
        string ret;
        ret.reserve(s.size() + spaces.size());
        int prev_i = 0;
        for (const int i : spaces) {
            copy(s.begin() + prev_i, s.begin() + i, back_inserter(ret));
            ret.push_back(' ');
            prev_i = i;
        }
        copy(s.begin() + prev_i, s.end(), back_inserter(ret));
        return ret;
    }
};
