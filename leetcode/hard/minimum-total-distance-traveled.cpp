// https://leetcode.com/problems/minimum-total-distance-traveled
// Dynamic programming + a bit of backtracking.
// Complexity: runtime O(r^2*f), space O(r*f) [where r == robot.size(),
//                                                   f == factory.size()].

class Solution {
    long long dfs(const vector<int> &rs, vector<vector<int>> &fs,
                  array<pair<long long, int>, 100 * 100> &mem,
                  const int repairs_left, const int ri = 0,
                  const int fi = 0) const
    {
        if (repairs_left < rs.size() - ri)          // impossible; return max
            return 2 * 1'000'000'000ll * rs.size(); // result for handling min()
        if (ri >= rs.size())
            return 0;

        const size_t memi = rs.size() * fi + ri;
        if (const auto [d, limit] = mem[memi]; fs[fi][1] <= limit)
            return d;

        auto d = dfs(rs, fs, mem, repairs_left - fs[fi][1], ri, fi + 1);
        if (fs[fi][1] > 0) {
            --fs[fi][1];
            d = min(d, abs(rs[ri] - fs[fi][0])
                           + dfs(rs, fs, mem, repairs_left - 1, ri + 1, fi));
            ++fs[fi][1]; // back-track
        }
        mem[memi] = {d, fs[fi][1]};
        return d;
    }

public:
    long long minimumTotalDistance(vector<int> &robot,
                                   vector<vector<int>> &factory) const
    {
        sort(robot.begin(), robot.end());
        sort(factory.begin(), factory.end());
        const int total_repairs =
            accumulate(factory.begin(), factory.end(), 0,
                       [](const int a, const auto &v) { return a + v[1]; });

        array<pair<long long, int>, 100 * 100> mem;
        mem.fill({-1, -1});
        return dfs(robot, factory, mem, total_repairs);
    }
};
