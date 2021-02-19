// https://leetcode.com/problems/arithmetic-slices
//
// See code comments for details.
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int numberOfArithmeticSlices(const vector<int>& A) const noexcept
    {
        int result = 0;

        for (size_t i = 0; i + 1 < size(A);) {
            const int expectedDiff = A[i + 1] - A[i];
            size_t j = i + 2;
            for (; j < size(A) && A[j] - A[j - 1] == expectedDiff; ++j) {
                // A valid arithmetic slice is any arithmetic slice in A with
                // length >= 3. This means that, for any valid arithmetic slice
                // with length >= 4, there also exists (maybe many) arithmetic
                // sub-slices of at least length 3 in that slice. For each time
                // we're able to increase the length of our valid slice from [i,
                // j], there will be (j - i - 1) additional valid arithmetic
                // sub-slices (in other words, if our slice is of length N,
                // there will be (N - 1) additional sub-slices whenever N is
                // incremented).
                result += j - (i + 1);
            }

            // Start the next search at the last valid element in the previously
            // found arithmetic slice, if any (a different valid slice may begin
            // with this number; e.g: in [1, 2, >3<, 5, 7], the value 3 is the
            // last element in the valid slice [1, 2, 3], but is also valid in
            // the slice [3, 5, 7].
            i = j - 1;
        }

        return result;
    }
};
