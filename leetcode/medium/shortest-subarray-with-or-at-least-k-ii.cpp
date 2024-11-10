// https://leetcode.com/problems/shortest-subarray-with-or-at-least-k-ii
// Faster than 100%
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int minimumSubarrayLength(const vector<int> &nums, const int k) const
    {
        array<int, (sizeof k) * CHAR_BIT> bit_freq{};
        int ret = numeric_limits<int>::max();

        for (int l = 0, curr_or = 0, n = 0;;) {
            if (curr_or < k || n < 2) {
                if (l + n >= nums.size())
                    break;

                unsigned x = nums[l + n++];
                curr_or |= x;
                for (unsigned i = 0; x; ++i, x >>= 1)
                    bit_freq[i] += x & 1;
            } else {
                --n;
                for (unsigned i = 0, x = nums[l++]; x; ++i, x >>= 1) {
                    bit_freq[i] -= x & 1;
                    if (bit_freq[i] < 1)
                        curr_or &= ~(1 << i);
                }
            }

            if (curr_or >= k)
                ret = min(ret, n);
        }

        return ret == numeric_limits<int>::max() ? -1 : ret;
    }
};
