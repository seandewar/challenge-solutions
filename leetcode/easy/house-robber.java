// https://leetcode.com/problems/house-robber/
//
// Iterative solution with dynamic programming using constant space. It is a
// bottom-up version of the alternative recursive solution below.
//
// For every house, we have 2 possible choices:
//  1. we rob this house, which means we can't rob the adjacent houses - this
//     means we couldn't have robbed the previous house to this one prior.
//  2. we don't rob this house.
//
// Due to these constraints, if we were to rob a house, we must have *not*
// robbed the previous house, and thus, we have the same amount of cash we had
// while considering the previous house. Therefore, we need to store our
// current 'cash' as a variable, but also a variable 'cashIfDidntRobPrevHouse'
// that is equal to our previous amount of cash. To consider if it is worth
// robbing the house being considered, we test whether or not we'd have more
// cash robbing this house (which means we "rollback" any robbery of the
// previous house we may have considered via 'cashIfDidntRobPrevHouse +
// houseCash') or by ignoring it (which may have involved a robbery of the
// previous house).
//
// This is functionally equivalent to the alternative iterative solution below
// utilizing a memoization array instead; if you look closely, you'll notice
// that the other solution only ever uses resultCache[i-1] and resultCache[i],
// meaning that only 2 variables are ever needed, which this solution uses and
// names appropriately.
//
// Complexity: runtime O(n), space O(1).

class Solution {
    public int rob(final int[] nums) {
        int cash = 0;
        int cashIfDidntRobPrevHouse = 0;

        for (final int houseCash : nums) {
            final int newCashIfRobbing = cashIfDidntRobPrevHouse + houseCash;

            cashIfDidntRobPrevHouse = cash;
            cash = Math.max(cash, newCashIfRobbing);
        }

        return cash;
    }
}

// Alternative Solution: Iterative solution with dynamic programming
// (memoization array version).
//
// Complexity: runtime O(n), space O(n).
/*
class Solution {
    public int rob(final int[] nums) {
        if (nums.length == 0)
            return 0;

        final var resultCache = new int[nums.length + 1];
        resultCache[1] = nums[0];

        for (int i = 1; i < nums.length; ++i) {
            resultCache[i + 1] = Math.max(resultCache[i - 1] + nums[i],
                                          resultCache[i]);
        }

        return resultCache[nums.length];
    }
}
*/

// Alternative Solution: Recursive solution with dynamic programming.
// Complexity: runtime O(n), space O(n).
/*
class Solution {
    private int rob(final int[] nums, final int i, final int[] resultCache) {
        if (i < 0)
            return 0;

        if (resultCache[i] == -1) { // result uncached - calculate it
            resultCache[i] = Math.max(rob(nums, i - 2, resultCache) + nums[i],
                                      rob(nums, i - 1, resultCache));
        }

        return resultCache[i];
    }

    public int rob(final int[] nums) {
        var resultCache = new int[nums.length];
        Arrays.fill(resultCache, -1); // sentinel value -1 = uncached result

        return rob(nums, nums.length - 1, resultCache);
    }
}
*/
