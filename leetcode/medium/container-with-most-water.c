/* https://leetcode.com/problems/container-with-most-water */
/*  */
/* Brute Force; surprisingly, this seems to pass for C, at least. */
/* Complexity: runtime O(n^2), space O(1). */

int maxArea(const int* const height, const int heightSize)
{
    int maxArea = 0;

    for (const int* h1 = height; h1 < height + heightSize; ++h1) {
        for (const int* h2 = h1 + 1; h2 < height + heightSize; ++h2) {
            const int minHeight = *h1 < *h2 ? *h1 : *h2;
            const int area = minHeight * (h2 - h1);
            maxArea = maxArea > area ? maxArea : area;
        }
    }

    return maxArea;
}
