// https://leetcode.com/problems/create-sorted-array-through-instructions
//
// Binary Indexed Tree (Fenwick Tree) Solution.
// See the Rust version of this solution for more details.
//
// Complexity: runtime O(n*logn), space O(1).

class Solution {
public:
    int createSortedArray(const vector<int>& instructions) const noexcept
    {
        uint_fast64_t result = 0;
        array<int, 100'001> bit{};

        const auto bitSum = [&bit](int x) {
            int sum = 0;
            for (; x != 0; x -= x & -x)
                sum += bit[x];

            return sum;
        };
        const auto bitInc = [&bit](int x) {
            for (; x < size(bit); x += x & -x)
                ++bit[x];
        };

        for (size_t i = 0; i < size(instructions); ++i) {
            result += min<int>(bitSum(instructions[i] - 1),
                               i - bitSum(instructions[i]));
            bitInc(instructions[i]);
        }

        return result % 1'000'000'007;
    }
};
