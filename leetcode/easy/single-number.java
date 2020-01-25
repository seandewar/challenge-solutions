// https://leetcode.com/problems/single-number/
//
// Uses XOR to find the number that isn't repeated twice in the array.
//
// Simply put, because XOR is commutative (a^b == b^a), a^a == 0, 0^a == a and
// a^b^a == b, and that each element appears twice in the array except one, the
// element that only appears once will be the final result of the XORing
// process.
//
// This solution uses Java 8 streams, but is functionally equivalent to:
//
// int result = 0;
//
// for (int x : nums)
//     result ^= x;
//
// return result;
//
// This can also be easily solved using a hash table, for example, in O(n) time
// (but with O(n) extra space).
//
// Complexity: runtime O(n), space O(1).

class Solution {
    public int singleNumber(final int[] nums) {
        return Arrays.stream(nums).reduce(0, (a, b) -> a ^ b);
    }
}
