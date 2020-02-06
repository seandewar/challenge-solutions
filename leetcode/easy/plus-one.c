// https://leetcode.com/problems/plus-one/
//
// Complexity: runtime O(n), space O(1) [output array (O(n)) not considered in
// complexity calculations].

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
// NOTE: assumes all arguments are valid & non-empty (e.g: not NULL) and that
// the malloc() & realloc() always succeeds to reduce the line count. this is
// fine as all the test inputs are valid
int* plusOne(const int* const restrict digits,
             const int digitsSize,
             int* const restrict returnSize)
{
    int* result = malloc(digitsSize * sizeof(int));
    bool carry = true;

    for (int i = digitsSize - 1; i >= 0; --i) {
        result[i] = digits[i];

        if (carry) {
            if (++result[i] == 10)
                result[i] = 0;
            else
                carry = false;
        }
    }

    *returnSize = digitsSize;

    if (carry) {
        result = realloc(result, ++*returnSize * sizeof(int));
        result[*returnSize - 1] = 0;
        result[0] = 1;
    }

    return result;
}
