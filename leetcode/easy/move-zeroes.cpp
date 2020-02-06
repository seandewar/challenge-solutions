// https://leetcode.com/problems/move-zeroes/
//
// Question asks us to move the 0s to the end of the array, while maintaining
// the order of the other elements. This has to be done in-place, we are not
// allowed to make a copy of the array.
//
// This approach uses the standard library's stable_partition() algorithm,
// which moves elements where the predicate given returns true before the
// elements where the predicate returns false, preserving their relative
// orders. This is pretty much the perfect fit for this problem!
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    auto moveZeroes(vector<int>& nums) const noexcept -> void
    {
        stable_partition(begin(nums), end(nums),
                         [](const int x) { return x != 0; });
    }
};

// Alternative Solution: 2 iterators approach (1 is abstracted by the
// range-based for loop).
//
// Complexity: runtime O(n), space O(1).
/*
class Solution {
public:
    auto moveZeroes(vector<int>& nums) const noexcept -> void
    {
        auto newPosIt{begin(nums)};

        for (const int x : nums) {
            if (x != 0)
                *(newPosIt++) = x;
        }

        fill(newPosIt, end(nums), 0);
    }
};
*/

// Alternative Solution: individually select and swap 0s with all elements
// in-front (to maintain order) until it reaches the end of the array.
//
// Complexity: runtime O(n^2), space O(1).
/*
class Solution {
public:
    auto moveZeroes(vector<int>& nums) const noexcept -> void
    {
        for (auto rit{rbegin(nums)}; rit != crend(nums); ++rit) {
            if (*rit != 0)
                continue;

            // NOTE: reverse_iterator dereferences to the position before its
            // base iterator, so we -1 from rit.base() when initializing it
            for (auto it{rit.base() - 1}; it != cend(nums) - 1; ++it) {
                if (*(it + 1) == 0) // we can terminate early if we meet a 0
                    break;          // (we've met the chain of 0s at the end)

                iter_swap(it, it + 1);
            }
        }
    }
};
*/
