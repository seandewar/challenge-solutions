// https://leetcode.com/problems/implement-strstr/
//
// Standard library string::find() solution.
//
// Complexity: No idea, standard has no guarantees here.
//
// An implementation may use Knuth-Morris-Pratt (described as an alternative
// solution below), but I'm going to assume some will use a different algorithm
// due to the O(n) space overhead that comes with KMP.

class Solution {
public:
    auto strStr(const string& haystack, const string& needle) const -> int
    {
        const auto i{haystack.find(needle)};
        return i != string::npos ? i : -1;
    }
};

// Alternative Solution: brute-force.
//
// This solution, for each index i in haystack, tries to match needle with a
// substring of equal size to needle starting at index i of haystack. If any
// character fails to match, the matching is aborted and has to start from the
// beginning for the next index i (back-tracking). This is repeated until a
// match is found or we run out of characters in haystack to perform a full
// match [i>=size(haystack)-size(needle)+1].
//
// The back-tracking results a runtime complexity of O(m*n), but this can be
// eliminated using the KMP algorithm instead (see alternative solution below).
//
// Complexity: runtime O(m*n), space O(1).
/*
class Solution {
public:
    auto strStr(const string& haystack, const string& needle) const -> int
    {
        // NOTE: casts used to avoid implicit conversion to unsigned, which
        // allows -1 to be returned if size(needle) > size(haystack).
        //
        // size(needle) is subtracted from [size(haystack) + 1] to allow us to
        // terminate early if the number of remaining chars in haystack is less
        // than size(needle) (match isn't possible anymore if this is the case)

        const auto searchSize{static_cast<int>(size(haystack))
                              - static_cast<int>(size(needle)) + 1};

        for (int i{}; i < searchSize; ++i) {
            for (int j{}; ; ++j) {
                if (j == size(needle))
                    return i;

                if (haystack[i + j] != needle[j])
                    break; // back-track
            }
        }

        return -1;
    }
};
*/

// Alternative Solution: Knuth-Morris-Pratt (KMP) string search algorithm.
//
// To preface, it's unlikely you'll ever be asked about or need to remember
// this algorithm - it's a lot to take in, but somewhat simple to understand
// how it works when it clicks (it might help looking up some videos on the
// topic, which might be simpler to understand than my comments).
//
// KMP is more efficient than a brute-force approach, as it avoids
// back-tracking by completing the search in a single traversal of haystack.
//
// If, while searching for needle in haystack, we have managed to partially
// match needle with some substring [i, j] in haystack (in other words, we have
// matched this haystack substring with another substring starting at index 0
// of needle with the same length), and there exists a non-overlapping prefix
// [i, p] that is equal to a suffix [s, j] of the substring, then there is also
// a smaller overlapping partial match between needle and haystack at substring
// [s, j] in particular. This is important, as if there is no longer a partial
// match with needle after trying to match the next character in haystack
// (substring [i, j + 1]), we may instead try to continue matching needle with
// substring [s, j + 1] of haystack instead, which can be done without
// back-tracking our progress traversing haystack.
//
// To implement this, a "failure table" is used. This table is the same size as
// needle, with each entry at index i referring to the length of the longest
// non-overlapping prefix in a substring [0, i] of needle that equals a suffix
// in the same substring. If a mismatch between a substring of haystack and
// needle occurs at index j of needle, the match can instead try to continue
// from index table[j]-1 of needle, unless table[j]==0, which indicates there's
// no overlapping match to try to continue from (but there may be one later in
// haystack).
//
// See code comments for some more details.
//
// Complexity: runtime O(m+n), space O(m) [failure table].
/*
class Solution {
    // creates a table containing at each index i the lengths of the longest
    // non-overlapping prefix that is also a suffix of needle when substring'd
    // from index 0 to i+1 inclusive
    auto constructFailureTable(const string& needle) const -> vector<int>
    {
        if (empty(needle))
            return {};

        vector<int> result;
        result.reserve(size(needle));
        result.push_back(0); // size 1 substr has no non-overlapping pre==suffix

        // NOTE: j refers to the last char of a needle prefix that we are
        // checking is a valid non-overlapping suffix of needle substring'd
        // from index 0 to size(result) inclusive.
        //
        // if, after expanding our prefix by 1 char (or after shrinking it due
        // to a previously failed equivalence check) there is no equivalent
        // non-overlapping suffix & j!=0, then we use similar logic explained
        // in strStr() (see below) when j!=0 to determine if there is a smaller
        // non-overlapping prefix==suffix whose length we can record instead by
        // shrinking the prefix and trying again. if there is no such
        // pre==suffix, then j==0, and we record 0 to indicate as such
        for (int j{}; size(result) < size(needle);) {
            if (needle[size(result)] == needle[j])
                result.push_back(++j);
            else if (j > 0)
                j = result[j - 1];
            else
                result.push_back(0);
        }

        return result;
    }

public:
    auto strStr(const string& haystack, const string& needle) const -> int
    {
        if (empty(needle))
            return 0;

        const auto failureTable{constructFailureTable(needle)};

        for (int i{}, j{}; i < size(haystack);) {
            if (haystack[i] == needle[j]) {
                ++i;

                if (++j >= size(needle))
                    return i - size(needle);
            } else {
                // if j!=0, then there was previously a partial match with
                // needle. we check our failure table to see if there was
                // another, shorter partial match with needle (which is the
                // case when the partial match contains a non-overlapping
                // prefix that is also a suffix). the table entry at j-1 (the
                // index of the last char in needle that matched) will contain
                // the length of the longest prefix==suffix of our partial
                // match, which we set as the new position in needle to match
                // against.
                //
                // this step removes the need for back-tracking when a match
                // fails as is the case with a brute force approach, for
                // example

                if (j > 0)
                    j = failureTable[j - 1];
                else
                    ++i;
            }
        }

        return -1;
    }
};
*/
