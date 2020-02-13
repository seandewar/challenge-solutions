// https://leetcode.com/problems/longest-common-prefix/
//
// Complexity: runtime O(sum of number of chars in input strings), space O(1)
// [output not considered in complexity analysis].

class Solution {
    public String longestCommonPrefix(final String[] strs) {
        if (strs.length == 0)
            return "";

        final int minStrLength = Arrays.stream(strs)
                                 .mapToInt(s -> s.length()).min().orElseThrow();
        int prefixLength = 0;

        search:
        for (; prefixLength < minStrLength; ++prefixLength) {
            for (int i = 1; i < strs.length; ++i) {
                if (strs[0].charAt(prefixLength)
                    != strs[i].charAt(prefixLength))
                    break search;
            }
        }

        return strs[0].substring(0, prefixLength);
    }
}

// Alternative Solution: Less lines without using Arrays.stream(), but same
// idea otherwise.
//
// Complexity: runtime O(sum of number of chars in input strings), space O(1)
// [output not considered in complexity analysis].
/*
class Solution {
    public String longestCommonPrefix(final String[] strs) {
        if (strs.length == 0)
            return "";

        for (int prefixLen = 0; prefixLen < strs[0].length(); ++prefixLen) {
            for (int i = 1; i < strs.length; ++i) {
                if (prefixLen >= strs[i].length()
                    || strs[0].charAt(prefixLen) != strs[i].charAt(prefixLen))
                    return strs[i].substring(0, prefixLen);
            }
        }

        return strs[0];
    }
}
*/
