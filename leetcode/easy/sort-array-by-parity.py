# https://leetcode.com/problems/sort-array-by-parity/
#
# In-place solution.
# Complexity: runtime O(n), space O(1).

class Solution:
    def sortArrayByParity(self, A: List[int]) -> List[int]:
        i, j = 0, len(A) - 1

        while i < j:
            if A[i] % 2 == 0:
                i += 1
            else:
                A[j], A[i] = A[i], A[j]
                j -= 1

        return A
