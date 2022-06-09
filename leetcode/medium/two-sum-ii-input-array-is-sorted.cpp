// https://leetcode.com/problems/two-sum-ii-input-array-is-sorted
//
// Two pointers.
//
// As the array is sorted, we know that if *l + *r is larger than target, we
// need to --r, and ++l if the opposite is true.
//
// Note that it's not necessary to use this upper_bound/lower_bound bisect stuff
// (++l, --r works), but it's slightly faster as it skips over equal ranges
// faster.
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    vector<int> twoSum(const vector<int> &numbers, const int target) const
    {
        for (auto l = cbegin(numbers), r = prev(cend(numbers)); r > l;) {
            if (const int sum = *l + *r; sum == target) {
                return {static_cast<int>(distance(cbegin(numbers), l)) + 1,
                        static_cast<int>(distance(cbegin(numbers), r)) + 1};
            } else if (sum < target) {
                l = upper_bound(l, cend(numbers), *l);
            } else {
                r = prev(upper_bound(reverse_iterator(next(r)), crend(numbers),
                                     *r, greater())
                             .base());
            }
        }
        abort();
    }
};

// Alternative Solution: Binary search.
// Complexity: runtime O(nlogn), space O(1).

class Solution {
public:
    vector<int> twoSum(const vector<int> &numbers, const int target) const
    {
        for (auto it = cbegin(numbers); it != cend(numbers);
             it = upper_bound(it, cend(numbers), *it)) {
            const int need = target - *it;
            const auto it2 = lower_bound(next(it), cend(numbers), need);
            if (it2 != cend(numbers) && *it2 == need) {
                return {static_cast<int>(distance(cbegin(numbers), it) + 1),
                        static_cast<int>(distance(cbegin(numbers), it2) + 1)};
            }
        }
        abort();
    }
};
