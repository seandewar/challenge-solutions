// https://leetcode.com/problems/reordered-power-of-2
//
// Count the digits in N. For every power of 2 less than 1 << 31 (or more
// generally, 1 << (sizeof(int) * CHAR_BIT - 1), the max power of 2
// representable in a signed int), count the digits, and compare them with those
// we counted in N.
//
// If the counts are equal, then it's possible to re-arrange the digits in N to
// create this power of 2; return true. If this isn't the case for any of our
// powers of 2, we return false.
//
// Complexity: runtime O((logn)^2), space O(1).

class Solution {
public:
    bool reorderedPowerOf2(unsigned N) const
    {
        array<unsigned, 10> digCounts{};
        for (; N != 0; N /= 10)
            ++digCounts[N % 10];

        for (N = 1; N < (1 << 31); N <<= 1) {
            array<unsigned, 10> digCounts2{};
            for (auto N2 = N; N2 != 0; N2 /= 10)
                ++digCounts2[N2 % 10];

            if (digCounts == digCounts2)
                return true;
        }

        return false;
    }
};

// Alternative Solution: Generate and test all permutations of N.
// Complexity: runtime O((logn)!*logn), space O(1).

class Solution {
public:
    bool reorderedPowerOf2(int N) const
    {
        vector<unsigned char> digits;
        while (N != 0)
            digits.push_back(exchange(N, N / 10) % 10);

        sort(begin(digits), end(digits));
        do {
            if (digits[0] == 0)
                continue;

            N = 0;
            for (auto d : digits)
                N = N * 10 + d;

            if (bitset<(sizeof N) * CHAR_BIT>(N).count() == 1)
                return true;
        } while (next_permutation(begin(digits), end(digits)));

        return false;
    }
};
