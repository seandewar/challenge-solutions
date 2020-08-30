// https://leetcode.com/problems/find-right-interval/
//
// Standard library std::sort() and std::lower_bound() solution.
// Complexity: runtime O(n*logn), space O(n).

class Solution {
public:
    vector<int> findRightInterval(vector<vector<int>>& intervals) const
    {
        // we're gonna sort, so we need to save the original interval indices
        // for the result; this is an easy way of doing that
        for (int i = 0; i < size(intervals); ++i) {
            intervals[i].push_back(i);
        }

        sort(begin(intervals), end(intervals),
             [](const auto& a, const auto& b) { return a[0] < b[0]; });

        vector<int> result(size(intervals));

        for (const auto& x : intervals) {
            const auto fIt = lower_bound(
                cbegin(intervals), cend(intervals), x,
                [](const auto& a, const auto& b) { return a[0] < b[1]; });

            result[x[2]] = fIt != cend(intervals) ? (*fIt)[2] : -1;
        }

        return result;
    }
};

// Alternative Solution: Manual binary search + std::sort().
// Complexity: runtime O(n*logn), space O(n).

class Solution {
public:
    vector<int> findRightInterval(vector<vector<int>>& intervals) const
    {
        for (int i = 0; i < size(intervals); ++i) {
            intervals[i].push_back(i);
        }

        sort(begin(intervals), end(intervals),
             [](const auto& a, const auto& b) { return a[0] < b[0]; });

        vector<int> result(size(intervals));

        for (const auto& x : intervals) {
            size_t left = 0, right = size(intervals);

            while (left < right) {
                const auto middle = left + (right - left) / 2;

                if (intervals[middle][0] >= x[1]) {
                    right = middle;
                } else {
                    left = middle + 1;
                }
            }

            result[x[2]] = right < size(intervals) ? intervals[right][2] : -1;
        }

        return result;
    }
};
