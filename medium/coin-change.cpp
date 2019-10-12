// https://leetcode.com/problems/coin-change/
//
// Dynamic Programming Iterative Solution (bottom-up)
// Complexity: runtime O(m*n), space O(m).

class Solution {
public:
    int coinChange(vector<int>& coins, int amount)
    {
        vector<int> fewestCache(amount + 1);

        for (int a = 1; a <= amount; ++a) {
            int minCoins = -1;

            for (const int coin : coins) {
                if (coin <= a) {
                    const int cached = fewestCache[a - coin];

                    // ensure it was possible to make the exact amount from this
                    // current coin total
                    if (cached != -1) {
                        minCoins = minCoins != -1 ? min(cached + 1, minCoins)
                                                  : cached + 1;
                    }
                }
            }

            fewestCache[a] = minCoins;
        }

        return fewestCache[amount];
    }
};

/*
// Dynamic Programming Recursive Solution (depth-first search, top-down)
// Complexity: runtime O(m*n), space O(m).

class Solution {
    int fewestCoins(const vector<int>& coins, int target,
                    vector<int>& fewestCache)
    {
        if (target == 0) {
            return 0;
        }
        if (target < 0) {
            return -1;
        }

        const int cacheIdx = target - 1;

        // we've already calculated the fewest to make this target, or we've
        // already determined it's impossible. return that value instead
        if (fewestCache[cacheIdx] != 0) {
            return fewestCache[cacheIdx];
        }

        int minCoins = -1;

        for (const int coin : coins) {
            const int result = fewestCoins(coins, target - coin, fewestCache);

            if (result >= 0) {
                minCoins = minCoins >= 0 ? min(minCoins, result + 1)
                                         : result + 1;
            }
        }

        return fewestCache[cacheIdx] = minCoins;
    }

public:
    int coinChange(vector<int>& coins, int amount)
    {
        vector<int> fewestCache(amount);
        return fewestCoins(coins, amount, fewestCache);
    }
};
*/
