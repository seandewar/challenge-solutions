// https://leetcode.com/problems/best-sightseeing-pair
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int maxScoreSightseeingPair(const vector<int> &values) const
    {
        int ret = 0;
        for (int best = values.back() - (values.size() - 1),
                 i = values.size() - 2;
             i >= 0; --i) {
            ret = max(ret, values[i] + i + best);
            best = max(best, values[i] - i);
        }
        return ret;
    }
};
