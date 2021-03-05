// https://leetcode.com/problems/divisor-game
//
// Somewhat similar to the idea used for my Nim Game solution.
// Complexity: runtime O(1), space O(1).

class Solution {
public:
    bool divisorGame(const int N) const noexcept { return N % 2 == 0; }
};
