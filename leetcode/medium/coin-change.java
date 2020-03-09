// https://leetcode.com/problems/coin-change/
//
// See my C++ Coin Change solutions for more information on this problem.
//
// Both of these solutions use a hash map for the cache, but you can use a data
// structure such as an array of size amount+1 or similar instead.
//
// Iteration + dynamic programming (bottom-up approach).
// Complexity: runtime O(n*m), space O(m).

class Solution {
    public int coinChange(final int[] coins, final int amount) {
        final var cache = new HashMap<Integer, Integer>();
        cache.put(0, 0); // 0 coins needed to reach 0 amount from 0 amount

        for (int i = 0; i < amount; ++i) {
            final var fewest = cache.get(i);

            if (fewest == null)
                continue;

            for (final int c : coins) {
                final var currFewestAtNext = cache.get(i + c);

                if (currFewestAtNext == null || currFewestAtNext > fewest + 1)
                    cache.put(i + c, fewest + 1);
            }
        }

        return cache.containsKey(amount) ? cache.get(amount) : -1;
    }
}

// Alternative Solution: Recursion + dynamic programming (top-down approach).
// Complexity: runtime O(n*m), space O(m).
/*
class Solution {
    private int coinChange(final int[] coins, final int amount,
                           final Map<Integer, Integer> cache) {
        if (amount <= 0)
            return amount == 0 ? 0 : -1;

        // we may have computed the min number of coins required to reach 0 from
        // this amount before - use it if we have
        if (cache.containsKey(amount))
            return cache.get(amount);

        int minFewest = Integer.MAX_VALUE;

        for (final int c : coins) {
            final int nextFewest = coinChange(coins, amount - c, cache);

            if (nextFewest != -1)
                minFewest = Math.min(minFewest, nextFewest + 1);
        }

        cache.put(amount, minFewest != Integer.MAX_VALUE ? minFewest : -1);
        return cache.get(amount);
    }

    public int coinChange(final int[] coins, final int amount) {
        final var cache = new HashMap<Integer, Integer>();
        return coinChange(coins, amount, cache);
    }
}
*/
