// https://leetcode.com/problems/sum-of-two-integers/
//
// Complexity: runtime O(logn), space O(1).

class Solution {
    public int getSum(int a, int b) {
        while (b != 0) { // nothing to add (or no more carry remains to sum)
            final int carry = a & b; // compute the carry for each bit
            a ^= b; // sum a and b without considering the carry
            b = carry << 1; // next, sum carry with next significant bits
        }

        return a;
    }
}
