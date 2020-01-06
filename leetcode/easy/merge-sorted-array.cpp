// https://leetcode.com/problems/merge-sorted-array/
//
// Merges 2 sorted arrays in-place (inside nums1), sorting the largest elements
// to the end of the array first in order to do so. Reuses array sizes m & n as
// indices because we can.
//
// Complexity: runtime O(m + n), space O(1).

class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n)
    {
        while (n > 0) {
            // may be tempting to write this as a single ternary expression:
            //
            // nums1[m + n - 1] = m > 0 && nums1[m - 1] > nums2[n - 1]
            //     ? nums1[--m]
            //     : nums2[--n];
            //
            // but this is undefined behaviour. the compiler may choose to
            // evaluate [m + n - 1] AFTER the ternary, evaluating [--m] or
            // [--n] first instead!

            if (m > 0 && nums1[m - 1] > nums2[n - 1]) {
                --m;
                nums1[m + n] = nums1[m];
            } else {
                --n;
                nums1[m + n] = nums2[n];
            }
        }
    }
};
