// https://leetcode.com/problems/duplicate-zeros/
//
// Fun pre-C++20 Standard Library solution using std::rotate().
// If you have access to C++20, you can use std::shift_right() instead!
// Or you can cheese this by using std::vector::insert() and erase().
//
// This solution is mostly for fun; the optimal solution can be done in just
// O(n) imperatively.
//
// Complexity: runtime O(n^2), space O(1).

class Solution {
public:
    void duplicateZeros(vector<int>& arr) const noexcept
    {
        for (auto it = begin(arr); it != end(arr); ++it) {
            if (*it == 0) {
                rotate(rbegin(arr), rbegin(arr) + 1, reverse_iterator(it));

                if (it + 1 != cend(arr)) {
                    *(it++) = 0;
                }
            }
        }
    }
};
