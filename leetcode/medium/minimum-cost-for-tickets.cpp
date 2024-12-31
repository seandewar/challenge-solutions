// https://leetcode.com/problems/minimum-cost-for-tickets
// Dynamic programming, bottom-up, iterative solution.
// Complexity: runtime O(max_day), space O(max_day).

class Solution {
public:
    int mincostTickets(const vector<int> &days, const vector<int> &costs) const
    {
        array<int, 366> memo;
        memo[0] = 0;

        for (int prev_d = 0; const auto d : days) {
            fill(memo.begin() + prev_d + 1, memo.begin() + d, memo[prev_d]);

            const auto cost_1 = memo[max(0, d - 1)] + costs[0],
                       cost_7 = memo[max(0, d - 7)] + costs[1],
                       cost_30 = memo[max(0, d - 30)] + costs[2];
            memo[d] = min(min(cost_1, cost_7), cost_30);

            prev_d = d;
        }

        return memo[days.back()];
    }
};
