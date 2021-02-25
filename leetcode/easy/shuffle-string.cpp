// https://leetcode.com/problems/shuffle-string
//
// In-place swapping; no extra-space.
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    string restoreString(string& s, vector<int>& indices) const noexcept
    {
        for (size_t i = 0; i < size(s);) {
            if (i == indices[i]) {
                ++i;
                continue;
            }

            swap(s[i], s[indices[i]]);
            swap(indices[i], indices[indices[i]]);
        }

        return s;
    }
};

// Alternative Solution: Use another string.
// Complexity: runtime O(n), space O(n).

class Solution {
public:
    string restoreString(const string& s, const vector<int>& indices) const
    {
        string result(size(s), '\0');
        for (size_t i = 0; i < size(s); ++i)
            result[indices[i]] = s[i];

        return result;
    }
};
