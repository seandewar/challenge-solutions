// https://leetcode.com/problems/roman-to-integer/
//
// Probably one of the simpler solutions.
//
// Will throw NullPointerException from value >= nextValue if nextValue is null
// due to an invalid character in the input. However, to reduce complexity,
// some invalid numerals (e.g IIV) will produce a result, as the test inputs
// are always valid numerals.
//
// Complexity: runtime O(n), space O(1).

class Solution {
    // NOTE: java's Map.of() creates an immutable Map from a number of
    // key+value pairs.  Map.of() is overloaded to allow for up to 10 pairs,
    // use Map.ofEntries() if you want more
    private static final Map<Character, Integer> ROMAN_MAP = Map.of(
        'I', 1,
        'V', 5,
        'X', 10,
        'L', 50,
        'C', 100,
        'D', 500,
        'M', 1000);

    public int romanToInt(final String s) {
        int result = 0;

        for (int i = 0; i < s.length(); ++i) {
            final var value = ROMAN_MAP.get(s.charAt(i));
            final var nextValue = i < s.length() - 1
                ? ROMAN_MAP.get(s.charAt(i + 1))
                : 0;

            // if numeral to the right has a higher value, assume subtraction
            // (e.g: IV=4, IX=9, XL=40), as under normal circumstances, smaller
            // numerals exist to the right of larger ones
            result += value * (value >= nextValue ? 1 : -1);
        }

        return result;
    }
}
