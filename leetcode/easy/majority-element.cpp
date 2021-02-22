// https://leetcode.com/problems/majority-element/
//
// In-place std::nth_element().
//
// If we sort the input, then the n/2th element will always be the majority
// value, as the majority value will occupy (n/2)+1 elements of nums, and thus,
// must occupy the middle position of the sorted input regardless of where the
// sequence of duplicates start within it.
//
// std::nth_element() will partially sort the input, sorting the requested
// nth-element in the position it would be if the whole range was sorted.
//
// Note that if there is no majority element in the input, this returns an
// arbitrary value, however, the test inputs are always guaranteed to contain a
// majority element for this problem.
//
// Complexity: runtime O(n) [O(n*logn) or higher worst case], likely space O(1)
// or O(logn), depending on nth_element() implementation.

class Solution {
public:
    int majorityElement(vector<int>& nums) const
    {
        nth_element(begin(nums), begin(nums) + size(nums) / 2, end(nums));
        return nums[size(nums) / 2];
    }
};

// Alternative Solution: counting using a hash map.
// Complexity: runtime O(n), space O(n).
/*
class Solution {
public:
    int majorityElement(const vector<int>& nums) const
    {
        unordered_map<int, vector<int>::size_type> counts;

        for (const int x : nums) {
            if (++counts[x] > size(nums) / 2)
                return x;
        }

        return -1; // impossible; majority guaranteed in inputs
    }
};
*/

// Alternative Solution: Boyer-Moore majority vote algorithm.
//
// This algorithm works by thinking of our input as only containing two types of
// elements: majority and non-majority elements. For example, given:
//
// [2, 2, 1, 2, 1, 3, 2]
//
// ...where 2 is the majority element, we can instead think of this as:
//
// [M, M, X, M, X, X, M]
//
// ...where M = majority element, X = non-majority element.
//
// As you can see, the majority element will *always* be the modal element of
// the input. Under these circumstances, if we were to select some element in
// our input as the possible majority while maintaining a counter which, while
// traversing the input, we increment for each element we see that is equal to
// our candidate element, but decrement for each element that is not equal to
// our candidate, the counter will be positive past the end of the input if the
// candidate we selected was indeed the majority element, and negative if not.
//
// Using this logic, we can instead change our candidate majority to the current
// value being traversed in the input if our counter is zero rather than
// decrementing it to a negative number if the current value is not equal to our
// candidate. This will have the effect of our candidate being the actual
// majority value in the input once we finish traversing it.
//
// Note that, if there exists no majority in the input, this algorithm will
// *not* return the modal (most common) value in the input (the value returned
// is arbitrary), however, the test cases guarantee a majority always exists in
// the input.
//
// Complexity: runtime O(n), space O(1).
/*
class Solution {
public:
    int majorityElement(const vector<int>& nums) const
    {
        vector<int>::size_type counter = 0;
        int possibleMajority; // will be init'd in loop (unless empty(nums))

        for (const int x : nums) {
            if (x == possibleMajority)
                ++counter;
            else if (counter == 0)
                possibleMajority = x;
            else
                --counter;
        }

        // undefined if empty(nums) & arbitrary if no majority exists in nums
        return possibleMajority;
    }
};
*/

// Alternative Solution: In-place sort and iterate through the sorted array,
// finding the value that has duplicates of length size(nums)/2+1.
//
// Complexity: runtime O(n*logn), likely space O(1) or O(logn), depending on
// sort() implementation.
/*
class Solution {
public:
    int majorityElement(vector<int>& nums) const
    {
        sort(begin(nums), end(nums));

        for (auto it = cbegin(nums), eIt = it; it != cend(nums); ++it) {
            if (*it != *eIt)
                eIt = it;
            else if (it - eIt + 1 > size(nums) / 2)
                return *it;
        }

        return -1; // impossible; majority guaranteed in inputs
    }
};
*/

// Alternative Solution: In-place sort and std::find_if_not() (variation of the
// above alternative solution).
//
// Complexity: runtime O(n*logn), likely space O(1) or O(logn), depending on
// sort().
/*
class Solution {
public:
    int majorityElement(vector<int>& nums) const
    {
        sort(begin(nums), end(nums));
        auto prevIt = cbegin(nums);

        while (true) { // terminates eventually as majority guaranteed in inputs
            const auto it = find_if_not(prevIt, cend(nums), *prevIt);

            if (it - prevIt > size(nums) / 2)
                return *prevIt;

            prevIt = it;
        }
    }
};
*/
