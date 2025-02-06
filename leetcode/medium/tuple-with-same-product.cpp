// https://leetcode.com/problems/tuple-with-same-product
// Complexity: runtime O(n^2), space O(n^2).

class Solution {
public:
    int tupleSameProduct(const vector<int> &nums) const
    {
        unordered_map<int, int> freqs;
        for (int i = 0; i + 1 < nums.size(); ++i)
            for (int j = i + 1; j < nums.size(); ++j)
                freqs[nums[i] * nums[j]] += 2; // pairs (a, b) and (b, a)

        return ranges::fold_left(freqs, 0, [](const int acc, const auto &kv) {
            const int freq = kv.second;
            return acc + freq * (freq - 2); // pair (a, b) also ignores (b, a)
        });
    }
};
