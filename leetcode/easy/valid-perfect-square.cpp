// https://leetcode.com/problems/valid-perfect-square/
//
// The sum of consecutive positive odd numbers will be a perfect square. Using
// this, we can keep summing these numbers until we either equal or surpass
// "num"; if we equal it, "num" is a perfect square, otherwise it's not.
//
// Complexity: runtime O(sqrt(n)), space O(1).

class Solution {
public:
    bool isPerfectSquare(int num) const
    {
        for (int n = 1; num > 0; n += 2)
            num -= n;

        return num == 0;
    }
};

// Alternative Solution: binary search.
//
// Basically the same approach as the below one, but using a binary search...
// You can probably also solve this using Newton-Raphson.
//
// Complexity: runtime O(log(n)), space O(1).

class Solution {
public:
    bool isPerfectSquare(const int num) const
    {
        int left = 0, right = num;

        while (right >= left) {
            const int middle = left + (right - left) / 2;
            const long square = static_cast<long>(middle) * middle;

            if (square == num)
                return true;
            else if (square < num)
                left = middle + 1;
            else // square > num
                right = middle - 1;
        }

        return false;
    }
};

// Alternative Solution: linear (bruteforce) search.
//
// This solution searches all perfect squares from 0 leading up to or surpassing
// "num".
//
// Complexity: runtime O(sqrt(n)), space O(1).

class Solution {
public:
    bool isPerfectSquare(const int num) const
    {
        int i = 0;
        for (; static_cast<long>(i) * i < num; ++i); // long > int on leetcode;
        return static_cast<long>(i) * i == num;      // handles the INT_MAX case
    }
};
