// https://leetcode.com/problems/kth-largest-element-in-an-array/
//
// In-place std::nth_element().
//
// Same as using std::sort(), except nth_element() can terminate earlier without
// needing to sort the entire input (partial sort), making its average time
// complexity O(n). At the very least, all elements before the new nth-element
// will be <= the new nth-element (however, that doesn't mean those elements
// will be completely sorted).
//
// Complexity: runtime O(n) [O(n*logn) or higher worst case], likely space O(1)
// or O(logn), depending on nth_element() impl.

class Solution {
public:
    int findKthLargest(vector<int>& nums, const int k) const
    {
        nth_element(begin(nums), begin(nums) + k - 1, end(nums),
                    greater<int>());
        return nums[k - 1];
    }
};

// Alternative Solution: In-place sorting using std::sort().
// Complexity: runtime O(n*logn), likely space O(1) or O(logn).
/*
class Solution {
public:
    int findKthLargest(vector<int>& nums, const int k) const
    {
        sort(begin(nums), end(nums), greater<int>());
        return nums[k - 1];
    }
};
*/

// Alternative Solution: Binary search.
//
// See my InterviewBit "Kth-Smallest Element in the Array" solution for more
// details on this solution.
//
// Complexity: runtime O(n*logm), space O(1).
/*
class Solution {
public:
    int findKthLargest(const vector<int>& nums, const int k) const
    {
        const auto [minIt, maxIt] = minmax_element(cbegin(nums), cend(nums));
        int min = *minIt, max = *maxIt;

        while (min <= max) {
            const int middle = min + (max - min) / 2;
            int numEquals = 0, numLarger = 0;

            for (const int x : nums) {
                numEquals += x == middle ? 1 : 0;
                numLarger += x > middle ? 1 : 0;
            }

            if (numLarger <= k - 1 && numLarger + numEquals >= k)
                return middle;
            else if (numLarger > k - 1)
                min = middle + 1;
            else
                max = middle - 1;
        }

        return 0; // impossible due to test cases
    }
};
*/
