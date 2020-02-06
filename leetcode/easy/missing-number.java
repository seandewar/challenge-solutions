// https://leetcode.com/problems/missing-number/
//
// There are many ways to approach this problem, especially without sorting and
// without using extra space. See the alternative solutions below for a few.
//
// Most of these solutions involve summing all numbers in the range [0, n] and
// all the numbers in the input, then finding the difference. The difference in
// sums will be the value of the missing number.
//
// This solution uses Gauss' summation formula [n*(n+1)/2] and an IntStream for
// summing the input.
//
// Complexity: runtime O(n), space O(1).

class Solution {
    public int missingNumber(final int[] nums) {
        return (nums.length * (nums.length + 1)) / 2 -
            Arrays.stream(nums).sum();
    }
}

// Alternative Solution: XORing.
//
// Because XOR is commutative, a^b^a==b, a^0==a and a^a==0 (XOR is the inverse
// of itself), XORing all numbers in the input against all numbers in the range
// [0, n] will leave 1 number in the range which had no corresponding number in
// the input to XOR against; because all other numbers were XOR'd against
// themselves, the number that remains is the number missing from the input.
//
// For example, given that b is missing from a set of elements {a, c, d}:
// d^a^a^c^b^d^c == (a^a)^(b)^(c^c)^(d^d) == 0^b^0^0 == b
//
// Complexity: runtime O(n), space O(1).
/*
class Solution {
    public int missingNumber(final int[] nums) {
        int result = nums.length;

        for (int i = 0; i < nums.length; ++i)
            result ^= i ^ nums[i];

        return result;
    }
}
*/

// Alternative Solution: Using 2 IntStreams. One calculates the expected sum,
// and the other calculates the actual sum.
//
// 2n runtime [range(0, n+1).sum() + stream(nums).sum()].
//
// Complexity: runtime O(n), space O(1).
/*
class Solution {
    public int missingNumber(final int[] nums) {
        return IntStream.rangeClosed(0, nums.length).sum() -
            Arrays.stream(nums).sum();
    }
}
*/
