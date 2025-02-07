// https://leetcode.com/problems/find-the-number-of-distinct-colors-among-the-balls
// Complexity: runtime O(q), space O(q).

class Solution {
public:
    vector<int> queryResults(const int limit,
                             const vector<vector<int>> &queries) const
    {
        unordered_map<int, int> balls, colour_counts;

        vector<int> ret;
        ret.reserve(queries.size());
        for (const auto &q : queries) {
            const int prev_colour = exchange(balls[q[0]], q[1]);
            if (prev_colour != 0 && --colour_counts[prev_colour - 1] <= 0)
                colour_counts.erase(prev_colour - 1);
            ++colour_counts[q[1] - 1];

            ret.push_back(colour_counts.size());
        }
        return ret;
    }
};
