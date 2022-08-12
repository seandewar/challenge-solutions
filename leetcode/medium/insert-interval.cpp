// https://leetcode.com/problems/insert-interval
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    vector<vector<int>> insert(vector<vector<int>> &intervals,
                               vector<int> &newInterval) const
    {
        auto it = lower_bound(begin(intervals), end(intervals), newInterval,
                              [](auto &a, auto &b) { return a[0] < b[0]; });
        it = intervals.insert(it, newInterval);
        auto itNew = it == begin(intervals) ? it : it - 1;
        for (auto itOld = itNew + 1; itOld != end(intervals); ++itOld) {
            if ((*itOld)[0] <= (*itNew)[1])
                (*itNew)[1] = max((*j)[1], (*itOld)[1]);
            else
                *(++itNew) = *itOld;
        }
        intervals.erase(itNew + 1, end(intervals));
        return intervals;
    }
};
