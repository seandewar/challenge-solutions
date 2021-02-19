// https://leetcode.com/problems/rotate-string
//
// Complexity: runtime O(n^2), space O(1).

class Solution {
public:
    auto rotateString(const string& A, const string& B) const noexcept -> bool
    {
        if (size(A) != size(B))
            return false;

        for (size_t i = 0; i < size(A); ++i) {
            size_t j = 0;
            for (; j < size(A); ++j) {
                if (A[(i + j) % size(A)] != B[j])
                    break;
            }

            if (j == size(A))
                return true; // rotated A matches with B
        }

        return empty(A); // only valid if A & B are empty
    }
};

// Alternative Solution: std::rotate() on A.
//
// Less efficient as we're actually mutating A to perform a full rotation each
// time.
//
// Complexity: runtime O(n^2), space O(1).

class Solution {
public:
    auto rotateString(string& A, const string& B) const noexcept -> bool
    {
        if (size(A) != size(B))
            return false;

        for (size_t i = 0; i < size(A) && A != B; ++i)
            rotate(begin(A), begin(A) + 1, end(A));

        return A == B;
    }
};
