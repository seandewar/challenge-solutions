// https://leetcode.com/problems/number-of-steps-to-reduce-a-number-to-zero
//
// Complexity: runtime O(1) [O(logn), but n is constrained to 32-bits], space
// O(1).

class Solution {
public:
    int numberOfSteps(int num) const noexcept
    {
        int steps = 0;
        for (; num != 0; ++steps)
            num = num % 2 == 0 ? num / 2 : num - 1;
        return steps;
    }
};
