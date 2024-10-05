// https://leetcode.com/problems/permutation-in-string
// Faster than 100%.
// Complexity: runtime O(n), space O(1).

bool checkInclusion(const char *const s1, const char *const s2)
{
    const int s1n = strlen(s1);
    if (s1n > strlen(s2)) // not required, but faster to check now
        return false;

    int counts[26] = {0};
    for (const char *c = s1; *c; ++c)
        ++counts[*c - 'a'];
    for (int i = 0; i < 26; ++i) {
        if (counts[i] == 0)
            counts[i] = INT_MIN; // letter not in s1 sentinel
    }

    int s1rem = s1n; // faster than checking all 26 slots in the freq table
    for (const char *c = s2; s1rem > 0 && *c; ++c) {
        if (counts[*c - 'a'] != INT_MIN && --counts[*c - 'a'] >= 0)
            --s1rem;
        if (c >= s2 + s1n && counts[*(c - s1n) - 'a'] != INT_MIN
            && ++counts[*(c - s1n) - 'a'] > 0)
            ++s1rem;
    }
    return s1rem == 0;
}
