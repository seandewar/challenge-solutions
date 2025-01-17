// https://leetcode.com/problems/neighboring-bitwise-xor
//
// A nice, easy medium.
// If derived is possible, each element in original would have been XOR'd twice.
// Therefore, XORing all numbers in derived should cancel-out to 0.
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    bool doesValidArrayExist(const vector<int> &derived) const
    {
        return ranges::fold_left(derived, 0, bit_xor{}) == 0;
    }
};
