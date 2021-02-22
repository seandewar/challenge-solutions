// https://leetcode.com/problems/intersection-of-two-arrays-ii/
//
// Uses 1 hash table.
// Complexity: runtime O(m+n), space O(m+n).

class Solution {
    public int[] intersect(final int[] nums1, final int[] nums2) {
        final var result = new ArrayList<Integer>();
        final var nums1Counts = new HashMap<Integer, Integer>();

        for (final int x : nums1)
            nums1Counts.put(x, nums1Counts.getOrDefault(x, 0) + 1);

        for (final int x : nums2) {
            if (!nums1Counts.containsKey(x))
                continue;

            if (nums1Counts.get(x) > 0)
                result.add(x);

            nums1Counts.put(x, nums1Counts.get(x) - 1);
        }

        // convert result Stream to IntStream, which we use to create an array
        // of unboxed ints to return. x -> x uses auto-unboxing, which is the
        // same as supplying Integer::intValue instead
        return result.stream().mapToInt(x -> x).toArray();
    }
}

// Alternative Solution: Sorting with window search.
// Complexity: runtime O(mlogm + nlogn), space likely O(logm + logn), depending
// on sort() implementation [O(m+n) if output considered, but we ignore it for
// complexity calculations].
/*
class Solution {
    public int[] intersect(final int[] nums1, final int[] nums2) {
        Arrays.sort(nums1);
        Arrays.sort(nums2);

        final var result = new ArrayList<Integer>();
        int nums1Idx = 0, nums2Idx = 0;

        while (nums1Idx < nums1.length && nums2Idx < nums2.length) {
            if (nums1[nums1Idx] == nums2[nums2Idx]) {
                result.add(nums1[nums1Idx]);

                ++nums2Idx;
                ++nums1Idx;
            } else if (nums1[nums1Idx] > nums2[nums2Idx]) {
                ++nums2Idx;
            } else {
                ++nums1Idx;
            }
        }

        return result.stream().mapToInt(x -> x).toArray();
    }
}
*/

// Alternative Solution: Using 2 hash tables.
// Complexity: runtime O(m+n), space O(m+n).
/*
class Solution {
    public int[] intersect(final int[] nums1, final int[] nums2) {
        final var nums1Counts = new HashMap<Integer, Integer>();

        for (final int x : nums1)
            nums1Counts.put(x, nums1Counts.getOrDefault(x, 0) + 1);

        final var intersectCounts = new HashMap<Integer, Integer>();

        for (final int x : nums2) {
            if (!nums1Counts.containsKey(x))
                continue;

            intersectCounts.put(x,
                                Math.min(intersectCounts.getOrDefault(x, 0) + 1,
                                         nums1Counts.get(x)));
        }

        final var result = new int[intersectCounts.values().stream()
                                   .mapToInt(x -> x).sum()];
        int startIdx = 0;

        for (final var kv : intersectCounts.entrySet()) {
            final int endIdx = startIdx + kv.getValue();

            Arrays.fill(result, startIdx, endIdx, kv.getKey());
            startIdx = endIdx;
        }

        return result;
    }
}
*/
