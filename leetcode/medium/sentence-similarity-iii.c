// https://leetcode.com/problems/sentence-similarity-iii
// Faster than 100%. See code comments.
// Complexity: runtime O(n+m), space O(1).

bool areSentencesSimilar(const char *const s1, const char *const s2)
{
    const size_t s1n = strlen(s1), s2n = strlen(s2);
    const char *const a0 = s1n > s2n ? s1 : s2,
                      *const b0 = s1n <= s2n ? s1 : s2,
                      *const a1 = a0 == s1 ? s1 + s1n : s2 + s2n,
                      *const b1 = a0 != s1 ? s1 + s1n : s2 + s2n;

    const char *a = a0, *b = b0, *bword = b;
    for (; *a && *b && *a == *b; ++a, ++b)
        if (*b == ' ')
            bword = b + 1;

    if (!*b && (!*a || *a == ' '))
        return true; // Exact or prefix match ending at a word boundary.

    // Mismatch. From the start of the mismatched word in b, get the prefix of a
    // with the same length, then check it starts at a word boundary and that
    // both strings match; if that fails, the strings can't be similar.
    if (b1 - bword > a1 - a)
        return false;

    a += a1 - a - (b1 - bword);
    return (a == a0 || *(a - 1) == ' ') && strcmp(a, bword) == 0;
}
