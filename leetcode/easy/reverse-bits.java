// https://leetcode.com/problems/reverse-bits/
//
// Standard library solution. Current implementation of Integer.reverse() is
// the same as the divide and conquer solution below.
//
// Complexity: runtime O(1), space O(1).

public class Solution {
    public int reverseBits(final int n) {
        return Integer.reverse(n);
    }
}

// Alternative Solution: In-place divide and conquer.
//
// Given a 32-bit pattern:
// abcdefghijklmnopqrstuvwxyzABCDEF
//
// The process works as follows:
//
// 1. divide the 32-bit chunk into 16-bit halves, swap adjacent halves:
// abcdefghijklmnop qrstuvwxyzABCDEF
// qrstuvwxyzABCDEF abcdefghijklmnop
//
// 2. divide the 16-bit chunks into 8-bit halves, swap adjacent halves:
// qrstuvwx yzABCDEF|abcdefgh ijklmnop
// yzABCDEF qrstuvwx|ijklmnop abcdefgh
//
// 3. divide the 8-bit chunks into 4-bit halves, swap adjacent halves:
// yzAB CDEF|qrst uvwx|ijkl mnop|abcd efgh
// CDEF yzAB|uvwx qrst|mnop ijkl|efgh abcd
//
// 4. divide the 4-bit chunks into 2-bit halves, swap adjacent halves:
// CD EF|yz AB|uv wx|qr st|mn op|ij kl|ef gh|ab cd
// EF CD|AB yz|wx uv|st qr|op mn|kl ij|gh ef|cd ab
//
// 5. divide the 2-bit chunks into 1-bit halves, swap adjacent halves:
// E F|C D|A B|y z|w x|u v|s t|q r|o p|m n|k l|i j|g h|e f|c d|a b
// F E|D C|B A|z y|x w|v u|t s|r q|p o|n m|l k|j i|h g|f e|d c|b a
//
// The result is a reversed bit pattern of the original.
// This also works if done in reverse order (step 5 to step 1 instead), and
// might be easier to understand this way, as in each stage, every swapped
// chunk will contain the reversed contents of what was originally there.
//
// Complexity: runtime O(1), space O(1).
/*
public class Solution {
    public int reverseBits(int n) {
        // NOTE: if we use >> (arithmetic/signed) shifts instead of >>>
        // (logical/unsigned) shifts, a sign extension will be performed,
        // which will screw up our answer

        n = (n << 16) | (n >>> 16);
        n = ((n & 0x00ff00ff) << 8) | ((n & 0xff00ff00) >>> 8);
        n = ((n & 0x0f0f0f0f) << 4) | ((n & 0xf0f0f0f0) >>> 4);
        n = ((n & 0x33333333) << 2) | ((n & 0xcccccccc) >>> 2);
        n = ((n & 0x55555555) << 1) | ((n & 0xaaaaaaaa) >>> 1);
        return n;
    }
}
*/

// Alternative Solution: Simple loop reversal.
// Complexity: runtime O(1), space O(1).
/*
public class Solution {
    public int reverseBits(int n) {
        int result = 0;

        for (int i = 0; i < 32; ++i) {
            result = (result << 1) | (n & 1);
            n >>>= 1; // using arithmetic shift (>>) instead wont affect result
        }

        return result;
    }
}
*/

// Alternative Solution: In-place divide and conquer (loop variant).
// Complexity: runtime O(1), space O(1).
/*
public class Solution {
    public int reverseBits(int n) {
        for (int chunkSize = 16; chunkSize > 0; chunkSize /= 2) {
            int mask = 0;
            for (int i = 0; i < 32; ++i) // calculate the chunk masking bits
                mask |= (i % (chunkSize * 2) < chunkSize ? 1 : 0) << i;

            n = ((n & mask) << chunkSize) | ((n & ~mask) >>> chunkSize);
        }

        return n;
    }
}
*/
