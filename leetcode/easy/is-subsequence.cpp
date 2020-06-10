// https://leetcode.com/problems/is-subsequence/
//
// Standard library solution.
// Complexity: runtime O(n+m), space O(1).

class Solution {
public:
    bool isSubsequence(const string& s, const string& t) const
    {
        for (auto tIt = cbegin(t); const char c : s) {
            tIt = find(tIt, cend(t), c);

            if (tIt++ == cend(t))
                return false;
        }

        return true;
    }
};

// Alternative Solution: manual, different iteration order.
// Complexity: runtime O(n+m), space O(1).

class Solution {
public:
    bool isSubsequence(const string& s, const string& t) const
    {
        if (empty(s))
            return true;

        for (auto sIt = cbegin(s); const char c : t) {
            if (*sIt == c && ++sIt == cend(s))
                return true;
        }

        return false;
    }
};
