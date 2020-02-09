// https://leetcode.com/problems/two-sum/
//
// Uses a hash map and 1 traversal of the input array.
//
// The map stores the ints in the input array paired with their respective
// indices. When we want to check if some other number would make the
// difference needed to meet our target, we simply refer to our map.
//
// Complexity: runtime O(n), space O(n).

class Solution {
    public int[] twoSum(final int[] nums, final int target) {
        final var indexMap = new HashMap<Integer, Integer>();

        for (int i = 0; i < nums.length; ++i) {
            if (indexMap.containsKey(target - nums[i]))
                return new int[] {indexMap.get(target - nums[i]), i};

            indexMap.put(nums[i], i);
        }

        return new int[0];
    }
}

// Alternative Solution: Hash map and 2 traversals of the input array.
//
// Unlike the 1 traversal approach, we need to check that the mapped number
// isn't referring to the same number we're looking at in nums, as we can't use
// the number at the same index in the input twice (although it is possible for
// the input to have duplicate numbers). We avoid this in the 1 pass approach
// by adding the number to the map after we've looked at the map for a number
// to make the difference.
//
// Complexity: runtime O(n), space O(n).
/*
class Solution {
    public int[] twoSum(final int[] nums, final int target) {
        final var indexMap = new HashMap<Integer, Integer>();
        for (int i = 0; i < nums.length; ++i)
            indexMap.put(nums[i], i);

        for (int i = 0; i < nums.length; ++i) {
            final var otherIdx = indexMap.get(target - nums[i]);

            if (otherIdx != null && otherIdx != i)
                return new int[] {i, otherIdx};
        }

        return new int[0];
    }
}
*/

// Alternative Solution: Brute-force.
// Complexity: runtime O(n^2), space O(1).
/*
class Solution {
    public int[] twoSum(final int[] nums, final int target) {
        for (int i = 0; i < nums.length; ++i) {
            for (int j = i + 1; j < nums.length; ++j) {
                if (nums[i] + nums[j] == target)
                    return new int[] {i, j};
            }
        }

        return new int[0];
    }
}
*/
