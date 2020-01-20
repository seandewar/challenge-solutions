// https://leetcode.com/problems/count-primes/
//
// Implements the Sieve of Eratosthenes; a faster method than the alternative
// solution listed at the end at the expense of linear space complexity. This
// is a pretty intuitive solution that is not too difficult to come up with by
// yourself: from the range [2, n), mark all larger multiples of each number as
// non-prime. Only the prime numbers will be left once you are finished.
//
// To optimize, we only consider numbers in the range [2, sqrt(n)) instead
// (while still marking the larger multiples of these numbers in the range [2,
// n) as non-prime). This is due that, given any n, there exists a p & q such
// that n = p*q, where p <= q; this means that p <= sqrt(n).
//
// It's possible to optimize this even further (the 2nd traversal of the primes
// vector via count() can be avoided, for example).
//
// Complexity: runtime O(n*log(logn)), space O(n).

class Solution {
public:
    int countPrimes(int n)
    {
        // vector denoting primeness of ints in range [2, n)
        vector<bool> primes(max(0, n - 2), true); // assume all prime for now

        for (int i{2}; i * i < n; ++i) {
            if (!primes[i - 2]) // if confirmed as non-prime, skip
                continue;

            // mark all subsequent multiples of i as non-prime, as they cannot
            // have only 1 and themselves as distinct factors (they also have i
            // as a factor). we only need to consider from i^2, as all previous
            // multiples would have been marked already
            for (int j{i * i}; j < n; j += i)
                primes[j - 2] = false;
        }

        return count(cbegin(primes), cend(primes), true);
    }
};

// Alternative Solution: sqrt() optimization only (good enough)
// Complexity: runtime O(n*sqrt(n) == n^1.5), space O(1).
/*
class Solution {
public:
    int countPrimes(int n)
    {
        int result{n > 2 ? 1 : 0}; // 2 is prime

        // += 2 as all even numbers > 2 aren't prime
        for (int i{3}; i < n; i += 2) {
            for (int j{3}; j * j <= i; j += 2) {
                if (i % j == 0)
                    goto notPrime;
            }

            ++result;

        notPrime:
            continue;
        }

        return result;
    }
};
*/
