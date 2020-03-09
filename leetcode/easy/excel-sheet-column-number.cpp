// https://leetcode.com/problems/excel-sheet-column-number/
//
// This is a simple problem, but do note that some of the test cases can invoke
// signed integer overflow (which is undefined behaviour) if you haven't
// considered the order of operations in depth. See my code comment for more
// details.
//
// Using std::accumulate().
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int titleToNumber(const string& s) const
    {
        // NOTE: order of operations is important here in avoiding overflow of
        // signed int, hence why we have brackets around (a*26) and (b-'A'+1) to
        // avoid overflow in the intermediate calculation (for example, if we
        // had no brackets, there's a possibility for signed overflow when
        // (a*26+b) is calculated before (char)'A' is subtracted!)
        return accumulate(cbegin(s), cend(s), 0,
            [](const int a, const char b) { return (a * 26) + (b - 'A' + 1); });
    }
};

// Alternative Solution: Explicit iteration.
// Complexity: runtime O(n), space O(1).
/*
class Solution {
public:
    int titleToNumber(const string& s) const
    {
        int result = 0;

        for (const char c : s)
            result = (result * 26) + (c - 'A' + 1);

        return result;
    }
};
*/

// Alternative Solution: Reverse iteration.
// Complexity: runtime O(n), space O(1).
/*
class Solution {
public:
    int titleToNumber(const string& s) const
    {
        int result = 0, offset = 1;

        for (auto rit = crbegin(s); rit != crend(s); ++rit) {
            // NOTE: calculating offset after result (or in the for loop's
            // iteration_expression: ++rit, offset*=26, for example) can lead to
            // signed overflow of offset, as we calculate the next value of
            // offset at the last iteration of the for loop even though it will
            // go unused. This will happen for test cases that produce a column
            // number close to INT_MAX, for example.
            //
            // Because we would like offset to be 1 for our right-most char, we
            // don't multiply by 26 when rit==crbegin(s)

            offset *= rit != crbegin(s) ? 26 : 1;
            result += (*rit - 'A' + 1) * offset;
        }

        return result;
    }
};
*/
