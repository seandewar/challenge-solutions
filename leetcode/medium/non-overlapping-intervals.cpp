// https://leetcode.com/problems/non-overlapping-intervals
//
// Complexity: runtime O(n*logn), likely space O(1) or O(logn), depending on
// sort().

class Solution {
public:
    int eraseOverlapIntervals(vector<vector<int>>& intervals) {
        if (empty(intervals))
            return 0;

        // sort ascending based on ending intervals. allows us to consider all
        // intervals sharing the same end before moving onto the intervals with
        // the next highest ends
        sort(begin(intervals), end(intervals),
             [](const auto& a, const auto& b) noexcept {
                 return a[1] < b[1];
             });

        int result = 0;
        int intervalEnd = intervals.front()[1];

        for (auto it = cbegin(intervals) + 1; it != cend(intervals); ++it) {

            // due to the sort, intervals with a beginning before our current
            // end will intersect with the interval whose end we're currently
            // storing in intervalEnd. we remove these intersecting intervals
            // until we find the first interval that doesn't intersect, storing
            // its end in intervalEnd to repeat the process from there (note
            // that intervals with start == end == intervalEnd don't really
            // intersect, thus shouldn't affect the current intervalEnd value)
            if ((*it)[0] < intervalEnd)
                ++result;
            else
                intervalEnd = (*it)[1];
        }

        return result;
    }
};
