// https://leetcode.com/problems/power-of-two/
//
// Standard library bit counting solution.
//
// Because binary digits are base 2, every binary (or base 2) order of magnitude
// is going to be a power of 2. As such, every power of 2 is represented with
// exactly 1 bit set in a binary number.
//
// Of course the standard recursion/iteration method also works, but I have not
// detailed that here.
//
// Complexity: runtime O(sizeof(int) * CHAR_BIT), space O(1).

class Solution {
public:
    bool isPowerOfTwo(const int n) const
    {
        return n > 0 && bitset<(sizeof n) * CHAR_BIT>(n).count() == 1;
    }
};

// Alternative Solution: Manual bit counting.
// Complexity: runtime O(sizeof(int) * CHAR_BIT), space O(1).

class Solution {
public:
    bool isPowerOfTwo(int n) const
    {
        if (n <= 0)
            return false;

        while (n) {
            // if bit 0 set in our shifted number, then number must be exactly 1
            // for this to be a power of 2; otherwise, multiple bits are set,
            // which means this number cannot be a power of 2
            if (n & 1 && n > 1)
                return false;

            n >>= 1; // no sign extension possible as guaranteed +ve here
        }

        return true;
    }
};

// Alternative Solution: Bit manipulation.
//
// This is a trick I've seen on Leetcode. The idea is the number before every
// order of magnitude will have all bits set other the single bit set as is the
// case with the next number (e.g: 3 in binary is 011, but 4 is 100); therefore,
// if we bitwise AND these two numbers together, the result should always be 0,
// as they share no set bits.
//
// Complexity: runtime O(1), space O(1).

class Solution {
public:
    bool isPowerOfTwo(const int n) const
    {
        return n > 0 && !(n & (n - 1));
    }
};
