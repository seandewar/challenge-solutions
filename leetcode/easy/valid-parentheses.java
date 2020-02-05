// https://leetcode.com/problems/valid-parentheses/
//
// Complexity: runtime O(n), space O(n).

class Solution {
    private static final Map<Character, Character> CLOSING_PAREN_MAP = Map.of(
        '(', ')',
        '[', ']',
        '{', '}');

    public boolean isValid(final String s) {
        final var stack = new ArrayDeque<Character>();

        for (int i = 0; i < s.length(); ++i) {
            final var closer = CLOSING_PAREN_MAP.get(s.charAt(i));

            if (closer != null)
                stack.push(closer); // push the corresponding closing parens
            else if (stack.size() == 0 || !stack.pop().equals(s.charAt(i)))
                return false; // wrong/no corresponding open parens
        }

        return stack.size() == 0; // eof. all parens should be closed by now
    }
}
