// https://leetcode.com/problems/add-digits/
//
// Digital root constant time solution.
// Complexity: runtime O(1), space O(1).

class Solution {
public:
    constexpr int addDigits(const int num) const noexcept
    {
        if (num == 0) {
            return 0;
        } else if (num % 9 == 0) {
            return 9;
        } else {
            return num % 9;
        }
    }
};

// Alternative Solution: Iteration.
// Complexity: runtime O(logn), space O(1).

class Solution {
public:
    constexpr int addDigits(int num) const noexcept
    {
        while (num > 9) {
            int tmp = num;
            num = 0;

            while (tmp > 0) {
                num += tmp % 10;
                tmp /= 10;
            }
        }

        return num;
    }
};
