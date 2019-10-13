// https://leetcode.com/problems/merge-intervals/
//
// See comments in solution for explaination of algorithm used.
// Complexity: runtime O(nlog(n)), space O(1).
//
// Runtime of the merging itself is O(n), but the sorting is slower, and thus,
// is the slowest part of the algorithm.
//
// Although O(n) worst case space is needed to create the output, we don't
// consider the input/output in space complexity calculations.

class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals)
    {
        if (empty(intervals)) {
            return {};
        }

        // sorting the intervals makes merging easier in one pass
        sort(begin(intervals), end(intervals),
             [](const vector<int>& a, const vector<int>& b) noexcept {
                 // no need to sort ends if starts are equal, as they will be
                 // merged anyway, so just consider the starts for sorting
                 return a[0] < b[0];
             });

        vector<vector<int>> results;
        pair merge(intervals.front()[0], intervals.front()[1]);

        for (auto it = cbegin(intervals) + 1; it != cend(intervals); ++it) {
            const pair interval((*it)[0], (*it)[1]);

            // because of sorted inputs, we only check if the end of the
            // merging interval intersects with the beginning of this one
            if (merge.second >= interval.first) {
                merge.second = max(merge.second, interval.second);
            }
            else {
                // if there's no intersection, we can't merge with this.
                // because the inputs have been sorted, this means that no
                // future intervals can be merged with our current merging
                // interval anymore! we commit our merge and start a new one
                results.push_back({merge.first, merge.second});
                merge = interval;
            }
        }

        results.push_back({merge.first, merge.second});
        return results;
    }
};
