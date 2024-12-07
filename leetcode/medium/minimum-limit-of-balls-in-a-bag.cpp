// https://leetcode.com/problems/minimum-limit-of-balls-in-a-bag
// Complexity: runtime O((logk)*n), space O(1) [where k == max(nums)].

#include <ranges> // LC has pretty good <ranges> support now!

class Solution {
public:
    int minimumSize(const vector<int> &nums, const int max_ops) const
    {
        return *ranges::lower_bound(
            views::iota(1, ranges::max(nums)), true, {},
            [&](const int max_num) {
                int ops = 0;
                return ranges::all_of(nums, [&](const int num) {
                    return (ops += (num - 1) / max_num) <= max_ops;
                });
            });
    }
};
