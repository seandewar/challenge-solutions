// https://leetcode.com/problems/numbers-with-same-consecutive-differences/
//
// Complexity: runtime O(k+n), space O(1).

class Solution {
    void buildResult(vector<int>& out, const int N, const int K,
                     const int digits, const int numDigits) const
    {
        if (numDigits == N) {
            out.push_back(digits);
            return;
        }

        const int rightDigit = digits % 10;

        if (rightDigit + K < 10) {
            buildResult(out, N, K, digits * 10 + rightDigit + K, numDigits + 1);
        }

        // when K is 0, there is only 1 digit to add (it's the same digit)
        if (K != 0 && rightDigit - K >= 0) {
            buildResult(out, N, K, digits * 10 + rightDigit - K, numDigits + 1);
        }
    }

public:
    vector<int> numsSameConsecDiff(const int N, const int K) const
    {
        vector<int> result;

        // the left-most digit will start at 1, so we do not have trailing 0s
        // for N > 1. if N = 1, then we need to start at 0 instead so 0 is
        // included in the output
        for (int digit = N > 1 ? 1 : 0; digit < 10; ++digit) {
            buildResult(result, N, K, digit, 1);
        }

        return result;
    }
};

// Alternative Solution: Using a vector to store the digits, constructing the
// number when its size is N.
//
// Complexity: runtime O(k+n), space O(n).

class Solution {
    void buildResult(vector<int>& out, const int N, const int K,
                     vector<int>& digits) const
    {
        if (digits.back() < 0 || digits.back() > 9) {
            return;
        }

        if (size(digits) == N) {
            int result = 0;

            for (const int d : digits) {
                result = (result * 10) + d;
            }

            out.push_back(result);
            return;
        }

        digits.push_back(digits.back() + K);
        buildResult(out, N, K, digits);

        // when K is 0, there is only 1 digit to add (it's the same digit)
        if (K != 0) {
            digits.back() -= 2 * K; // same as *(cend(digits) - 2) -= K
            buildResult(out, N, K, digits);
        }

        digits.pop_back(); // back-track
    }

public:
    vector<int> numsSameConsecDiff(const int N, const int K) const
    {
        vector<int> result;

        vector<int> digits;
        digits.reserve(N);

        // the left-most digit will start at 1, so we do not have trailing 0s
        // for N > 1. if N = 1, then we need to start at 0 instead so 0 is
        // included in the output
        for (digits.push_back(N > 1 ? 1 : 0); digits.back() < 10;
             ++digits.back()) {
            buildResult(result, N, K, digits);
        }

        return result;
    }
};
