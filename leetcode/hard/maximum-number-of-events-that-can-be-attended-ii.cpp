// https://leetcode.com/problems/maximum-number-of-events-that-can-be-attended-ii
//
// DFS + DP + bisects. k and n are small enough for us to pre-allocate a k*n
// size lookup table for memoization.
//
// Faster than 99.31%, less memory usage than 92.15%.
//
// Tl;dr: sort intervals by their starts, and DFS for the best sums by bisecting
// to and testing all other intervals that start *after* the end of the first.
// Memoize the best sums starting at each interval for the specific remaining
// value of k.
//
// Complexity: runtime O(k*n*logn), space O(k*n).

class Solution {
    int dfs(const auto &events, auto &memo, int k, int i)
    {
        const auto &event = events[i];
        if (k == 1) {
            return event[2];
        }
        if (memo[k][i] != -1) {
            return memo[k][i];
        }

        int best_sum = 0;
        for (auto it =
                 upper_bound(events.begin() + i + 1, events.end(), event[1],
                             [](int end, const auto &e) { return end < e[0]; });
             it != events.end(); ++it) {
            best_sum =
                max(best_sum, dfs(events, memo, k - 1, it - events.begin()));
        }
        return memo[k][i] = event[2] + best_sum;
    }

public:
    int maxValue(vector<vector<int>> &events, int k)
    {
        sort(events.begin(), events.end(),
             [](const auto &a, const auto &b) { return a[0] < b[0]; });

        vector memo(k + 1, vector(events.size(), -1));
        int result = 0;
        for (int i = 0; i < events.size(); ++i) {
            result = max(result, dfs(events, memo, k, i));
        }
        return result;
    }
};

// Alternative Solution: Same, but using a hash map for storing the
// memoizations (with a custom hash).
//
// Faster than 95.66%, less memory usage than 95.51%.
//
// Complexity: runtime O(k*n*logn), space O(k*n).

class Solution {
    struct MemoHash {
        size_t operator()(pair<int, int> p) const
        {
            return p.first | (static_cast<size_t>(p.second) << 32);
        }
    };
    using Memo = unordered_map<pair<int, int>, int, MemoHash>;
    using Events = vector<vector<int>>;

    int dfs(const Events &events, Memo &memo, int k, int i)
    {
        const auto &event = events[i];
        if (k == 1) {
            return event[2];
        }
        if (auto it = memo.find({k, i}); it != memo.end()) {
            return it->second;
        }

        int best_sum = 0;
        for (auto it =
                 upper_bound(events.begin() + i + 1, events.end(), event[1],
                             [](int end, const auto &e) { return end < e[0]; });
             it != events.end(); ++it) {
            best_sum =
                max(best_sum, dfs(events, memo, k - 1, it - events.begin()));
        }
        return memo[{k, i}] = event[2] + best_sum;
    }

public:
    int maxValue(Events &events, int k)
    {
        sort(events.begin(), events.end(),
             [](const auto &a, const auto &b) { return a[0] < b[0]; });

        Memo memo;
        int result = 0;
        for (int i = 0; i < events.size(); ++i) {
            result = max(result, dfs(events, memo, k, i));
        }
        return result;
    }
};
