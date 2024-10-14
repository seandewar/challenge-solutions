// https://leetcode.com/problems/maximal-score-after-applying-k-operations
//
// Very easy question for a medium. Using C++ this time to avoid reimplementing
// a binary heap in C for the *third* day in a row ("Smallest Range Covering
// Elements From K Lists", "Divide Intervals Into Minimum Number Of Groups" and
// "The Number Of The Smallest Unoccupied Chair").
//
// Complexity: runtime O(klogn + n) [heapification via the ctor is O(n)],
//             space O(1) [as we reuse the storage of nums; O(n) otherwise].

class Solution {
public:
    long long maxKelements(vector<int> &nums, const int k) const
    {
        priority_queue heap{less{}, std::move(nums)};
        long long score = 0;
        for (int i = 0; i < k; ++i) {
            const int v = heap.top();
            score += v;
            heap.pop();
            heap.push((v + 2) / 3);
        }
        return score;
    }
};
