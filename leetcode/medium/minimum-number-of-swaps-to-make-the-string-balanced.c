// https://leetcode.com/problems/minimum-number-of-swaps-to-make-the-string-balanced
// Complexity: runtime O(n), space O(1).

int minSwaps(const char *s)
{
    int need_open = 0, unmatched = 0;
    for (; *s; ++s) {
        need_open += *s == ']' ? 1 : -1;
        if (need_open > unmatched) // abs unneeded; max == -min for balancing to
            unmatched = need_open; // be possible, which is guaranteed by input
    }

    // At this point, unmatched is the count of out-of-place "]"s, and by
    // extension of balancing being possible at all, "["s too. An efficient swap
    // of one pair of these brackets fixes two pairs (the swapped "[" and "]"
    // may each find an unclosed "]" and "[" to pair with respectively), so
    // divide by two and ceil the result.
    return (unmatched + 1) / 2;
}
