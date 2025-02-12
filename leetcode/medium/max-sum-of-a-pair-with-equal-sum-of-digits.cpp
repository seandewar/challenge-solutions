// https://leetcode.com/problems/max-sum-of-a-pair-with-equal-sum-of-digits
// Complexity: runtime O(n+s), space O(s). [where s == max sum of digits]

class Solution {
public:
    int maximumSum(const vector<int> &nums) const
    {
        array<array<int, 2>, 9 * 9> bests{};
        for (const int v : nums) {
            int sum = 0;
            for (int x = v; x != 0; x /= 10)
                sum += x % 10;

            const auto it = ranges::min_element(bests[sum - 1]);
            *it = max(*it, v);
        }

        const auto &best =
            ranges::max(bests, [&](const auto &a, const auto &b) {
                if (ranges::find(a, 0) != a.end())
                    return true;
                if (ranges::find(b, 0) != b.end())
                    return false;

                return ranges::fold_left(a, 0, plus{})
                       < ranges::fold_left(b, 0, plus{});
            });
        return ranges::find(best, 0) == best.end()
                   ? ranges::fold_left(best, 0, plus{})
                   : -1;
    }
};
