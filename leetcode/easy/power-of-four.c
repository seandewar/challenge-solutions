// https://leetcode.com/problems/power-of-four/
//
// Complexity: runtime O(1), space O(1).

bool isPowerOfFour(const int num)
{
    // an integer is a power of 4 if it's higher than 0 and it has only a single
    // bit set in an odd position only (zero-indexed, where bit index 0 is
    // assumed odd)
    return num > 0
           && (num & 0x55555555)  // true if bit set in only odd positions
           && !(num & (num - 1)); // true if only a single bit is set
}
