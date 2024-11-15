// https://leetcode.com/problems/shortest-subarray-to-be-removed-to-make-array-sorted
//
// Faster than 100%; O(n) is possible by instead incrementally reducing the
// right-side search area one element at a time, but this performed better with
// the test cases (and we still re-use the updated starting index of the right
// side for the next iteration anyway).
//
// Complexity: runtime O(nlogn), space O(1).

class Solution {
public:
    int findLengthOfShortestSubarray(const vector<int> &arr) const
    {
        const auto unsort_l =
            is_sorted_until(arr.begin(), arr.end()) - arr.begin();
        if (unsort_l == arr.size())
            return 0;

        const auto unsort_r =
            arr.rend() - is_sorted_until(arr.rbegin(), arr.rend(), greater{})
            - 1;
        int ret = unsort_r + 1;
        for (int i = 0, j = unsort_r + 1; i < unsort_l; ++i) {
            j = lower_bound(arr.begin() + j, arr.end(), arr[i]) - arr.begin();
            ret = min(ret, j - i - 1);
        }
        return ret;
    }
};
