// https://leetcode.com/problems/find-building-where-alice-and-bob-can-meet
//
// 2 hard problems in a row before Christmas? Yikes!
//
// Complexity: runtime O(qlogq+qlogn+n), space O(n+q) [where q = queries.size(),
//                                                           n = heights.size()]

class Solution {
public:
    vector<int>
    leftmostBuildingQueries(const vector<int> &heights,
                            const vector<vector<int>> &queries) const
    {
        vector<int> query_is(queries.size()), ret(queries.size(), -1), stack;
        iota(query_is.begin(), query_is.end(), 0);
        sort(query_is.begin(), query_is.end(), [&](const auto i, const auto j) {
            const auto &a = queries[i], &b = queries[j];
            return max(a[0], a[1]) > max(b[0], b[1]);
        });

        for (int i = heights.size() - 1, qi = 0; i >= 0; --i) {
            for (; qi < query_is.size(); ++qi) {
                auto &out = ret[query_is[qi]];
                const auto &q = queries[query_is[qi]];
                const auto maxi = max(q[0], q[1]), mini = min(q[0], q[1]);
                if (heights[maxi] > heights[mini] || mini == maxi) {
                    out = maxi;
                    continue;
                }
                if (i > maxi)
                    break;

                const auto it = upper_bound(stack.rbegin(), stack.rend(),
                                            max(heights[q[0]], heights[q[1]]),
                                            [&](const auto maxh, const auto i) {
                                                return maxh < heights[i];
                                            });
                if (it != stack.rend())
                    out = *it;
            }

            while (!stack.empty() && heights[stack.back()] <= heights[i])
                stack.pop_back();
            stack.emplace_back(i);
        }

        return ret;
    }
};
