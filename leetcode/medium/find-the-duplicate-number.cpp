// https://leetcode.com/problems/find-the-duplicate-number/
//
// Uses Floyd's cycle-finding (Floyd's Tortoise & Hare) algorithm, as explained
// in the suggested solutions for this leetcode problem. This is the exact same
// solution for the "Linked List Cycle II" leetcode problem, just repurposed.
// (see my solution for that for in-depth explanation of the algorithm)
//
// If we interpret the listed numbers as indices to the next number in the
// list, this can be reduced to a cycle detection problem, as a repeated index
// will cause us to visit the same position in the list twice. Because 0 wont
// appear in the list, the number at index 0 wont be part of the cycle, hence
// we can't have a list such as [0, 1, 2] or [1, 0, 2] that contains no
// duplicates but still creates a cycle under our criteria.
//
// Using the algorithm under these rules, we have a "fast" and "slow" pointer,
// where the "slow" pointer traverses the list 1 step at a time and the "fast"
// pointer traverses 2 steps at a time. If both pointers eventually point to
// the same element in the list, we have a cycle.
//
// Unlikely this question will be asked with such constraints (see the leetcode
// page for a list of them) in a real interview as to expect this solution, see
// the alternative solutions below instead.
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int findDuplicate(vector<int>& nums)
    {
        // apply Floyd's & find the point where both indices refer to the same
        // position. this meeting point will be somewhere within the cycle
        int iFast = 0, iSlow = 0;
        do {
            iFast = nums[nums[iFast]];
            iSlow = nums[iSlow];
        } while (iFast != iSlow);

        // find where the cycle begins. see my "Linked List Cycle II" solution
        // for an explanation of how this works
        iFast = 0; // reuse as a 2nd slow index starting from the "list" head
        while (iFast != iSlow) {
            iFast = nums[iFast];
            iSlow = nums[iSlow];
        }

        return iSlow;
    }
};

// Alternative Solution: lookup table (hash set or array of n elements)
// (passes, but violates the "O(1) extra space" constraint)
//
// Complexity: runtime O(n), space O(n).
/*
class Solution {
public:
    int findDuplicate(vector<int>& nums)
    {
        unordered_set<int> seenNums;
        for (const int n : nums)
            if (seenNums.find(n) == cend(seenNums))
                seenNums.insert(n);
            else
                return n;

        assert(!"unreachable due to input constraints");
        return 0;
    }
};
*/

// Alternative Solution: sorting in-place (passes, but violates the "no
// modifying the input array" constraint)
//
// Complexity: runtime O(n*logn), space O(1).
/*
class Solution {
public:
    int findDuplicate(vector<int>& nums)
    {
        sort(begin(nums), end(nums));
        for (auto it = cbegin(nums) + 1; it != cend(nums); ++it)
            if (*it == *(it - 1))
                return *it;

        assert(!"unreachable due to input constraints");
        return 0;
    }
};
*/
