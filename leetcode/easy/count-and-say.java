// https://leetcode.com/problems/count-and-say/
//
// Complexity: runtime O(n*m), space O(n).

class Solution {
    public String countAndSay(final int n) {
        if (n == 1) // base case; test input guaranteed to be >= 1
            return "1";

        final var prev = countAndSay(n - 1);
        final var sb = new StringBuilder();

        for (int i = 0; i < prev.length();) {
            final int startIdx = i; // so we can determine the chain length

            // traverse chain of subsequent duplicate characters
            for (++i; i < prev.length(); ++i) {
                if (prev.charAt(startIdx) != prev.charAt(i))
                    break;
            }

            sb.append(i - startIdx); // write the length of the chain
            sb.append(prev.charAt(startIdx)); // write the chain's number
        }

        return sb.toString();
    }
}
