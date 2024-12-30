// https://leetcode.com/problems/count-ways-to-build-good-strings
//
// Tables are my favourite shape
// Cells filled where values meet
// Step by step, row by row, let's go
// My love, it's time to create
// 'Til solutions come
// Let's tabulate!
//
// (Based on the chorus of the song Tessellate by Alt-J)
//
// Complexity: runtime O(high), space O(high).

class Solution {
public:
    int countGoodStrings(const int low, const int high, const int zero,
                         const int one) const
    {
        vector<int> memo(high + 1);
        memo[0] = 1;

        constexpr auto mod = static_cast<int>(1e9) + 7;
        for (auto x = min(one, zero); x < memo.size(); ++x) {
            if (x >= one)
                memo[x] += memo[x - one];
            if (x >= zero)
                memo[x] += memo[x - zero];

            if (memo[x] >= mod)
                memo[x] %= mod;
        }

        return accumulate(memo.begin() + low, memo.end(), 0,
                          [mod](const auto acc, const auto sum) {
                              return (acc + sum) % mod;
                          });
    }
};
