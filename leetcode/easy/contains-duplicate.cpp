// https://leetcode.com/problems/contains-duplicate/
//
// Using sort() [O(n*logn)] and adjacent_find() [O(n)].
//
// The alternative solution actually has a better runtime complexity, but due
// to the test cases being used (nums is small), it performs worse for this
// leetcode question.
//
// Complexity: runtime O(n*logn), likely space O(1) or (logn) if quicksort, for
// example.

class Solution {
public:
    bool containsDuplicate(vector<int>& nums)
    {
        sort(begin(nums), end(nums));
        return adjacent_find(begin(nums), end(nums)) != cend(nums);
    }
};

// Alternative Solution: Using hash tables.
//
// Body can be replaced with:
//
// unordered_set<int> uniques;
//
// for (int n : nums) {
//     if (uniques.count(n) > 0)
//         return true;
//
//     uniques.insert(n);
// }
//
// return false;
//
// ...which is slightly faster than the solution below due to the early
// termination if a duplicate is found.
//
// Complexity: runtime O(n), space O(n).
/*
class Solution {
public:
    bool containsDuplicate(vector<int>& nums)
    {
        return size(unordered_set(cbegin(nums), cend(nums))) != size(nums);
    }
};
*/

// Alternative Cheese Solution: sort() with early termination... using
// exceptions...
//
// This feels pretty naughty. May just want to create your own sort() function
// that does this without exceptions...
//
// Complexity: runtime O(n*logn) [but without the extra O(n) from
// adjacent_find()], likely space O(1) or O(logn), depending on sort()
// implementation.
/*
class Solution {
public:
    bool containsDuplicate(vector<int>& nums)
    {
        try {
            sort(begin(nums), end(nums), [](int a, int b) {
                if (a == b)
                    throw runtime_error("duplicate found!");

                return a < b;
            });
        } catch (const runtime_error&) {
            return true;
        }

        return false;
    }
};
*/
