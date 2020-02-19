// https://leetcode.com/problems/letter-combinations-of-a-phone-number/
//
// Recursion (depth-first search) with backtracking using StringBuilder.
//
// Compared to the alternative solution at the end of this file, this solution
// uses a StringBuilder to build a single instance of the combination we are
// currently constructing (the picks variable). If we have finished constructing
// a combination (size(picks) == size(digits)), we copy it to our output.
//
// See code comments for more details.
//
// Complexity: runtime O(4^n), space O(n) [output not considered].

class Solution {
    private static final Map<Character, String> DIGIT_MAP = Map.of(
        '2', "abc",
        '3', "def",
        '4', "ghi",
        '5', "jkl",
        '6', "mno",
        '7', "pqrs",
        '8', "tuv",
        '9', "wxyz"
    );

    private void recurse(final String digits, final StringBuilder picks,
                         final List<String> out) {
        if (picks.length() >= digits.length()) {
            out.add(picks.toString());
            return;
        }

        final var digit = digits.charAt(picks.length());
        final var letters = DIGIT_MAP.get(digit);

        // append a temp char to increase pick's size. this allows us to set the
        // last char to each possible letter for this digit instead of deleting
        // the old char and re-appending for each loop iteration
        picks.append('\0');

        for (int j = 0; j < letters.length(); ++j) {
            // "soft" backtrack: replace the last char with the digit letter we
            // want to use instead for the next char of our combination
            picks.setCharAt(picks.length() - 1, letters.charAt(j));
            recurse(digits, picks, out);
        }

        // backtrack: remember to delete our end char so we can restore picks to
        // the original state we received it in from our caller
        picks.deleteCharAt(picks.length() - 1);
    }

    public List<String> letterCombinations(final String digits) {
        final var results = new ArrayList<String>();

        if (digits.length() > 0)
            recurse(digits, new StringBuilder(digits.length()), results);

        return results;
    }
}

// Alternative Solution: Recursion (depth-first search) with Strings.
//
// Because Strings are immutable, we have to create a new copy of the String of
// our current digit combination (picks & newPicks) for each new call to our our
// recurse() function. This is slower than the StringBuilder + backtracking
// solution.
//
// Complexity: runtime O(4^n), space O(n) [output not considered].
/*
class Solution {
    private static final Map<Character, String> DIGIT_MAP = Map.of(
        '2', "abc",
        '3', "def",
        '4', "ghi",
        '5', "jkl",
        '6', "mno",
        '7', "pqrs",
        '8', "tuv",
        '9', "wxyz"
    );

    private void recurse(final String digits, final String picks,
                         final List<String> out) {
        if (picks.length() >= digits.length()) {
            out.add(picks);
            return;
        }

        final var digit = digits.charAt(picks.length());
        final var letters = DIGIT_MAP.get(digit);

        for (int j = 0; j < letters.length(); ++j) {
            final var newPicks = picks + letters.charAt(j);
            recurse(digits, newPicks, out);
        }
    }

    public List<String> letterCombinations(final String digits) {
        final var results = new ArrayList<String>();

        if (digits.length() > 0)
            recurse(digits, "", results);

        return results;
    }
}
*/
