// https://leetcode.com/problems/string-to-integer-atoi/
//
// Tests for and handles overflow without using a larger data type than an int
// (signed 32-bits).
//
// Complexity: runtime O(n), space O(1).

class Solution {
    public int myAtoi(String str) {
        str = str.strip(); // strip leading and trailing white space

        boolean negative = false;
        int i = 0;

        // consume 1st sign symbol, if any. any other signs found afterwards
        // will terminate the calculation early (as expected by the question)
        if (str.length() > 0
            && (str.charAt(i) == '+' || str.charAt(i) == '-')) {
            negative = str.charAt(i) == '-';
            ++i;
        }

        int result = 0; // result treated as +ve during calculation

        for (; i < str.length(); ++i) {
            if (!Character.isDigit(str.charAt(i)))
                break;

            final var digit = str.charAt(i) - '0';

            // the below bools contain the overflow test results for the future
            // (result*10) and ([result*10]+digit) calculations respectively.
            //
            // NOTE: although not an underflow, a parsed -ve input of exactly
            // Integer.MIN_VALUE will cause the tests to pass and terminate the
            // calculation early (it would overflow our result variable, which
            // is represented +vely), however, this will not affect the final
            // result (Integer.MIN_VALUE will be returned anyway).
            final var mul10Overflow = result > Integer.MAX_VALUE / 10;
            final var addDigitOverflow = result == Integer.MAX_VALUE / 10
                                         && digit > Integer.MAX_VALUE % 10;

            if (mul10Overflow || addDigitOverflow)
                return negative ? Integer.MIN_VALUE : Integer.MAX_VALUE;

            result = result * 10 + digit;
        }

        if (negative) // result was treated +ve, don't forget to negate if -ve
            result *= -1;

        return result;
    }
}
