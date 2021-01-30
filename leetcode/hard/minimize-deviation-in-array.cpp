// https://leetcode.com/problems/minimize-deviation-in-array
//
// By doubling all odd nums, we turn them even; because even numbers cannot be
// doubled and must instead be halved, this even value will be the maximum value
// that the num can ever have.
//
// From this state, we only ever need to consider halving our array of even
// nums, calculating the new deviation after each halving of a num; if this new
// deviation is lower than our current lowest, we replace our current lowest
// with that value.
//
// If our new num is now odd after the halving, we need not consider it any
// more, as the only possible operation will be doubling it, which will end up
// increasing the deviation, or otherwise if it was initially odd, we'll end up
// wasting time on the same numbers we had previously divided again.
//
// Complexity: runtime O(n*(logn)*logm), space O(1) [nums is moved as heap
// storage; O(n) if not ignoring output] [where n = size(nums), m = max(nums) +
// 1].

class Solution {
public:
    auto minimumDeviation(vector<int>& nums) const noexcept -> int
    {
        for (int& x : nums)
            x *= (x % 2) + 1;

        const auto [minIt, maxIt] = minmax_element(cbegin(nums), cend(nums));
        int minVal = *minIt;
        int result = *maxIt - minVal;
        priority_queue<int> heap(less<int>(), move(nums));

        while (heap.top() % 2 == 0) {
            const int newVal = heap.top() / 2;
            const int newMinVal = min(minVal, newVal);
            heap.pop();
            heap.push(newVal);
            if (result >= heap.top() - newMinVal) {
                minVal = newMinVal;
                result = heap.top() - newMinVal;
            }
        }

        return result;
    }
};
