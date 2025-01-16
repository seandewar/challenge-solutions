// https://leetcode.com/problems/bitwise-xor-of-all-pairings
//
// Key observation is that XORing the same number twice cancels it out, so if
// there is an even number of pairings for every number in nums1, XOR nothing;
// otherwise just XOR them once (there will be nums2.size() pairings for each).
//
// Complexity: runtime O(n+m), space O(1).

class Solution {
public:
    int xorAllNums(const vector<int> &nums1, const vector<int> &nums2) const
    {
        const int nums2_xor = ranges::fold_left(nums2, 0, bit_xor{});
        return ranges::fold_left(nums1, 0, [&](const int acc, const int x) {
            return acc ^ ((nums2.size() % 2) * x) ^ nums2_xor;
        });
    }
};
