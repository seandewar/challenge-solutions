// https://leetcode.com/problems/merge-sorted-array/
//
// Complexity: runtime O(m + n), space O(1).

void merge(int *const restrict a1, int sz1, int m,
           const int *const restrict a2, const int sz2, int n)
{
    while (m || n) {
        a1[--sz1] = !n || m && a1[m - 1] > a2[n - 1] ? a1[--m] : a2[--n];
    }
}
