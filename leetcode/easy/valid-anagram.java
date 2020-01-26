// https://leetcode.com/problems/valid-anagram/
//
// Hash table method.
//
// This works for all 16-bit Unicode characters that the String might contain
// (Java chars are 16-bits, but can be used to represent larger Unicode
// characters when paired together, however this solution doesn't account for
// that).
//
// For inputs that only contain lower case characters (such as the test cases),
// an array of 26 elements is preferred instead [O(26)==O(1) space without the
// hash table overhead].
//
// Complexity: runtime O(n), space O(1) [specifically O(number of unique 16-bit
// Unicode characters), but O(65536) worst case].

class Solution {
    public boolean isAnagram(final String s, final String t) {
        if (s.length() != t.length())
            return false;

        final var charCounts = new HashMap<Character, Integer>();
        for (int i = 0; i < s.length(); ++i) {
            charCounts.put(s.charAt(i),
                           charCounts.getOrDefault(s.charAt(i), 0) + 1);
            charCounts.put(t.charAt(i),
                           charCounts.getOrDefault(t.charAt(i), 0) - 1);
        }

        return charCounts.values().stream().allMatch(v -> v == 0);
    }
}

// Alternative Solution: Sort and compare method.
//
// Complexity: runtime O(n*logn), space O(1) [toCharArray() is actually O(n)
// space, as it copies, but thats a Java-specific thing, so we ignore it].
/*
class Solution {
    public boolean isAnagram(final String s, final String t) {
        // not totally necessary, but we can exit early if the lengths differ,
        // as they cannot be anagrams if they do
        if (s.length() != t.length())
            return false;

        final var sChars = s.toCharArray();
        Arrays.sort(sChars);

        final var tChars = t.toCharArray();
        Arrays.sort(tChars);

        return Arrays.equals(sChars, tChars);
    }
}
*/
