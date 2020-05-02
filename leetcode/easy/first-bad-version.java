// https://leetcode.com/problems/first-bad-version/
//
// See my C++ solution for this problem for more details.
// Complexity: runtime O(logn), space O(1).

/* The isBadVersion API is defined in the parent class VersionControl.
      boolean isBadVersion(int version); */

public class Solution extends VersionControl {
    public int firstBadVersion(final int n) {
        int left = 1;
        int right = n;

        while (right > left) {
            final int middle = left + (right - left) / 2;

            if (isBadVersion(middle))
                right = middle;
            else
                left = middle + 1;
        }

        return right;
    }
}
