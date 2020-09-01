// https://leetcode.com/problems/largest-time-for-given-digits/
//
// Complexity: runtime O(1), space O(1).

class Solution {
public:
    string largestTimeFromDigits(vector<int>& A) const
    {
        sort(begin(A), end(A), greater());

        do {
            if (A[0] * 10 + A[1] <= 23 && A[2] <= 5) {
                ostringstream oss;
                oss << A[0] << A[1] << ':' << A[2] << A[3];
                return oss.str();
            }
        } while (prev_permutation(begin(A), end(A)));

        return {};
    }
};
