// https://leetcode.com/problems/guess-number-higher-or-lower/
//
// Uses binary search.
// Complexity: runtime O(log(n)), space O(1).

// Forward declaration of guess API.
// @param num, your guess
// @return -1 if my number is lower, 1 if my number is higher, otherwise return 0
int guess(int num);

class Solution {
public:
    int guessNumber(int n)
    {
        int r, g;
        int min = 1, max = n;

        while (g = min + (max - min) / 2, r = guess(g), r != 0) {
            if (r > 0) {
                min = g + 1;
            }
            else { // r < 0
                max = g - 1;
            }
        }

        return g;
    }
};
