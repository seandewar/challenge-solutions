// https://leetcode.com/problems/isomorphic-strings
//
// Using a single mapping array.
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    bool isIsomorphic(string s, string t) const
    {
        if (size(s) != size(t))
            return false;

        array<char, 128> m{};
        for (size_t i = 0; i < size(s); ++i) {
            if (char& v = m[s[i]]; v == 0)
                v = t[i];
            else if (v != t[i])
                return false;
        }

        // characters should be uniquely mapped
        sort(begin(m), end(m));
        auto it = upper_bound(cbegin(m), cend(m), 0);
        return adjacent_find(it, cend(m)) == cend(m);
    }
};
