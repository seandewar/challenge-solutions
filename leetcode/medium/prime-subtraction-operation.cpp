// https://leetcode.com/problems/prime-subtraction-operation
//
// Fun problem. Faster than 100%.
//
// Complexity: runtime O(nlogp), space O(p) [sieve generation time not included,
// as it's done at compile-time; where p is approx sqrt(max(nums))].

class Solution {
    constexpr static array<int, 1000> genPrimes()
    {
        array<bool, 1001> sieve{};
        sieve[0] = sieve[1] = true;
        for (int n = 2; n < sieve.size(); ++n) {
            for (int i = n * 2; i < sieve.size(); i += n)
                sieve[i] = true;
        }

        array<int, 1000> primes;
        int j = 0;
        for (int i = 1; i < sieve.size(); ++i) {
            if (!sieve[i])
                primes[j++] = i;
        }
        for (; j < primes.size(); ++j)
            primes[j] = numeric_limits<int>::max();
        return primes;
    }

public:
    bool primeSubOperation(const vector<int> &nums) const
    {
        constexpr auto primes = genPrimes();
        for (int prev = 0; int x : nums) {
            const auto it = lower_bound(primes.begin(), primes.end(), x - prev);
            if (it != primes.begin())
                x -= *(it - 1);
            if (prev >= x)
                return false;
            prev = x;
        }
        return true;
    }
};
