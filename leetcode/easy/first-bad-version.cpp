// https://leetcode.com/problems/first-bad-version/
//
// Performs a binary search to find the first bad version with the least amount
// of API calls. This is very similar to how "git bisect" works.
//
// Complexity: runtime O(logn), space O(1).

// Forward declaration of isBadVersion API.
bool isBadVersion(int version);

class Solution {
public:
    int firstBadVersion(int n)
    {
        int left{1}, right{n};

        while (left < right) {
            // avoids overflow compared to (left + right) / 2
            const int middle{left + (right - left) / 2};

            if (isBadVersion(middle))
                right = middle;
            else                   // 1 to middle are all good versions, so we
                left = middle + 1; // may safely adjust our search space by +1
        }

        // left will be the first bad version at this point. the input
        // guarantees there to be at least 1 bad version in the search space
        return left;
    }
};
