// https://leetcode.com/problems/minimum-number-of-removals-to-make-mountain-array
//
// Unlike my initial solution, what we only care about is the largest
// increasing/decreasing subsequence length starting/ending at any index in
// nums, not necessarily what number they end with (at least, outside of
// constructing the lookup structure).
//
// Complexity: runtime O(nlogn), space O(n).

class Solution {
    vector<int> getSubseqLens(auto it, const auto it1) const
    {
        vector<int> lens, min_vals;
        lens.reserve(it1 - it);
        min_vals.reserve(it1 - it);

        for (; it != it1; ++it) {
            if (const auto minit =
                    lower_bound(min_vals.begin(), min_vals.end(), *it);
                minit != min_vals.end())
                *minit = *it;
            else
                min_vals.push_back(*it);

            lens.push_back(min_vals.size());
        }
        return lens;
    }

public:
    int minimumMountainRemovals(const vector<int> &nums) const
    {
        const auto asc_lens = getSubseqLens(nums.begin(), nums.end()),
                   dsc_lens = getSubseqLens(nums.rbegin(), nums.rend());

        int max_len = 0;
        for (int i = 0; i < nums.size(); ++i) {
            if (const int j = nums.size() - 1 - i;
                asc_lens[i] > 1 && dsc_lens[j] > 1)
                max_len = max(max_len, asc_lens[i] + dsc_lens[j] - 1);
        }
        return nums.size() - max_len;
    }
};

// Alternative Solution: initial solution.
// Complexity: runtime O(n^2), space O(n).

class Solution {
public:
    int minimumMountainRemovals(const vector<int> &nums) const
    {
        map<int, vector<pair<int, int>>, greater<int>> asc_map, dsc_map;
        for (int i = 0; i < nums.size(); ++i) {
            const int x = nums[i];
            int n = 1;
            for (auto it = asc_map.upper_bound(x); it != asc_map.end(); ++it)
                n = max(n, it->second.back().second + 1);
            asc_map[x].emplace_back(i, n);
        }
        for (int i = nums.size() - 1; i >= 0; --i) {
            const int x = nums[i];
            int n = 1;
            for (auto it = dsc_map.upper_bound(x); it != dsc_map.end(); ++it)
                n = max(n, it->second.back().second + 1);
            dsc_map[x].emplace_back(i, n);
        }

        int max_len = 0;
        for (const auto &[x, v] : asc_map) {
            const auto it0 = dsc_map.upper_bound(x);
            if (it0 == dsc_map.end())
                continue;

            for (const auto [i, n] : v) {
                if (n < 2)
                    continue;

                for (auto it = it0; it != dsc_map.end(); ++it) {
                    const auto &v2 = it->second;
                    const auto pit =
                        upper_bound(v2.rbegin(), v2.rend(), pair{i, 0});
                    if (pit != v2.rend())
                        max_len = max(max_len, n + pit->second);
                }
            }
        }
        return nums.size() - max_len;
    }
};
