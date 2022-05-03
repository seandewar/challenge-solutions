// https://leetcode.com/problems/shortest-unsorted-continuous-subarray
//
// Complexity: runtime O(n*logn), space O(n).

class Solution {
public:
    int findUnsortedSubarray(const vector<int> &nums) const
    {
        auto sortedNums = nums;
        sort(begin(sortedNums), end(sortedNums));

        const auto leftIt =
            mismatch(cbegin(nums), cend(nums), cbegin(sortedNums)).second;

        if (leftIt == cend(sortedNums))
            return 0; // no mismatch (nums is already sorted)

        const auto rightIt =
            mismatch(crbegin(nums), crend(nums), crbegin(sortedNums)).second;

        // due to how reverse_iterators are implemented on-top of regular
        // bi-directional iters, this expression returns the distance between
        // both iters, plus 1 (which is what we want anyway)
        return rightIt.base() - leftIt;
    }
};

// Alternative Solution: Linear-time, constant-space solution.
//
// First, find the sorted range from [0, x) in nums (std::is_sorted_until()).
// If x equals size(nums), then nums is already sorted, and there's no work to
// do (return 0).
//
// Then, linearly scan the range [x-1, n), where n equals size(nums).
// While scanning, keep track of the minimum and maximum values we see in the
// range.
// If at any point our current maximum value is greater than our current scanned
// value, save the scan's position to rightIt.
// (rightIt's position will be the end of the shortest unsorted range in nums
// once our scan is complete)
//
// Binary search in the range [0, x) (std::upper_bound()) for the first value
// greater than the minimum we found for the range [x-1, n); save its position
// in leftIt.
// (leftIt's position will be the beginning of the shortest unsorted range in
// nums)
//
// Finally, to return the length of the shortest unsorted range, return rightIt
// - leftIt, plus one.
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int findUnsortedSubarray(const vector<int> &nums) const noexcept
    {
        auto it = is_sorted_until(cbegin(nums), cend(nums));
        if (it-- == cend(nums))
            return 0;

        int maxVal = *it, minVal = *it;
        auto leftIt = it, rightIt = it;
        for (++it; it != cend(nums); ++it) {
            if (*it < maxVal)
                rightIt = it;

            maxVal = max(maxVal, *it);
            minVal = min(minVal, *it);
        }

        leftIt = upper_bound(cbegin(nums), leftIt + 1, minVal);
        return (rightIt - leftIt) + 1;
    }
};

// Alternative Solution: full STL version of the above.
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int findUnsortedSubarray(const vector<int> &nums) const noexcept
    {
        auto it1 = is_sorted_until(cbegin(nums), cend(nums));
        if (it1 == cend(nums))
            return 0;

        auto it2 =
            is_sorted_until(crbegin(nums), crend(nums), greater()).base();
        const auto [minIt, maxIt] = minmax_element(prev(it1), next(it2));
        it1 = upper_bound(cbegin(nums), it1, *minIt);
        it2 = lower_bound(it2, cend(nums), *maxIt);
        return distance(it1, it2);
    }
};
