// https://www.interviewbit.com/problems/kth-smallest-element-in-the-array/
//
// This is trivial to solve if the input is mutable or if we could copy the
// input, but as we cannot do that, we're greatly restricted on what method we
// can use to solve this in an efficient manner.
//
// This solution uses binary searches to find the Kth-smallest element. Instead
// of performing the binary searches on the input itself, we perform it on the
// integers in the range [min_element(A), max_element(A)], counting the number
// of values in the input less than and the number of values equal to the middle
// integer in our binary search range.
//
// If there are exactly K-1 values less than the middle integer in our binary
// search range, then the middle integer is definitely the Kth-smallest value in
// the input! However, as it's possible to have duplicate values in our input,
// such a value can be the Kth-smallest value for multiple values of K (e.g
// given the input [2, 2, 4, 3], 2 is the Kth-smallest value for K=1 and 2).
// Otherwise, we adjust our binary search range: if the amount of values in the
// input that are less than the middle integer in the search range is larger
// than K-1, then the middle integer is too large, and our new range ends at
// middle-1, else, our middle is too small, and our new range starts at
// middle+1.
//
// Complexity: runtime O(n*logm), space O(1) [where n=size(A),
// m=max_element(A)-min_element(A)].

int Solution::kthsmallest(const vector<int>& A, const int K)
{
    if (A.empty()) // unreachable due to test inputs, but felt like including it
        throw invalid_argument("A cannot be empty");

    int min = *min_element(A.cbegin(), A.cend());
    int max = *max_element(A.cbegin(), A.cend());

    while (min <= max) {
        const int middle = min + (max - min) / 2;
        int lessThanCount = 0, equalToCount = 0;

        for (const int x : A) {
            lessThanCount += (x < middle ? 1 : 0);
            equalToCount += (x == middle ? 1 : 0);
        }

        if (K - 1 >= lessThanCount && K <= lessThanCount + equalToCount)
            return middle;
        else if (K - 1 < lessThanCount)
            max = middle - 1;
        else
            min = middle + 1;
    }

    // unreachable due to test inputs. the Kth-smallest can never be found if K
    // is <= 0 or > size(A) (K is not zero-indexed)
    throw invalid_argument("K is not in the range [1, size(A)]");
}
