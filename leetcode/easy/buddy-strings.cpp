// https://leetcode.com/problems/buddy-strings
//
// See code comments for details. Surprised this question is classed as Leetcode
// easy, as it does require a little bit of thinking; though it's not too hard.
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    auto buddyStrings(const string& A, const string& B) const noexcept -> bool
    {
        // This check isn't strictly necessary, but reduces runtime from O(n+m)
        // to O(2*n) == O(n).
        if (size(A) != size(B))
            return false;

        // Check whether A & B consist of the same characters; we can also
        // achieve this by sorting and comparing both strings, but we'll simply
        // count the occurrences of each letter, then check that both strings
        // have the same count for each letter. We also make note of whether or
        // not duplicate characters exist in A (or B) for later.
        array<size_t, 26> charCounts{};
        bool hasDuplicates = false;
        for (const char c : A)
            hasDuplicates |= ++charCounts[c - 'a'] > 1;
        for (const char c : B)
            --charCounts[c - 'a'];

        if (!all_of(cbegin(charCounts), cend(charCounts),
                    [](const auto n) { return n == 0; }))
            return false;

        // Count the number of characters in A that differ from the character at
        // the same index in B (or vice versa). If these strings are buddies,
        // there will be 2 characters somewhere in A or B that once swapped,
        // will make A & B equal.
        size_t numMismatches = 0;
        for (size_t i = 0; i < size(A); ++i) {
            if (A[i] != B[i] && ++numMismatches > 2)
                return false;
        }

        // We need to swap 2 characters, so we need 2 mismatches. Note however,
        // that we still need to perform a swap even if A & B are equal! Unless
        // A & B contain duplicate characters that we can swap, there will be no
        // way to swap anything without destroying their equality, ruining the
        // strings' friendship! Instead, by swapping a duplicate character, the
        // swap is essentially a no-op, leaving A & B's equality intact.
        return numMismatches == 2 || (hasDuplicates && numMismatches == 0);
    }
};
