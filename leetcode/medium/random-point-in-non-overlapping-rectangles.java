// https://leetcode.com/problems/random-point-in-non-overlapping-rectangles/
//
// See the alternative solution for a O(logn) runtime solution for pick().
//
// Complexity: init runtime O(n), pick() runtime O(n), space O(n).

class Solution {
    private final int[][] rects;
    private final int[] rectCumulAreas;

    private final Random rng = new Random();

    public Solution(final int[][] rects) {
        this.rects = rects;

        // first, populate with each rectangle's area (which includes the
        // perimeter, hence the +1 to the width and length calculations)
        rectCumulAreas = Arrays.stream(rects)
                               .mapToInt(rect -> (rect[2] - rect[0] + 1)
                                                 * (rect[3] - rect[1] + 1))
                               .toArray();

        // then, transform each area into a cumulation of the previous areas
        Arrays.parallelPrefix(rectCumulAreas, Integer::sum);
    }

    public int[] pick() {
        // find the first rectangle with a cumulative area larger than our
        // randomly, uniformly-picked value from the range [0, totalArea), and
        // pick a random point in its bounds; the cumulation stuff is necessary
        // so that larger rectangles have a higher chance of being picked in
        // proportion with how much bigger they are than the others
        final int totalArea = rectCumulAreas[rectCumulAreas.length - 1];
        final int areaPick = rng.nextInt(totalArea);

        final int recti = IntStream.range(0, rectCumulAreas.length)
                                   .filter(i -> areaPick < rectCumulAreas[i])
                                   .findFirst()
                                   .orElseThrow(IndexOutOfBoundsException::new);
        final var rect = rects[recti];

        final int x = rect[0] + rng.nextInt(rect[2] + 1 - rect[0]);
        final int y = rect[1] + rng.nextInt(rect[3] + 1 - rect[1]);

        return new int[] {x, y};
    }
}

/**
 * Your Solution object will be instantiated and called as such:
 * Solution obj = new Solution(rects);
 * int[] param_1 = obj.pick();
 */

// Alternative Solution: Binary search for the rectangle that was
// uniformly-picked based on its area.
//
// Complexity: init runtime O(n), pick() runtime O(logn), space O(n).

class Solution {
    private final int[][] rects;
    private final int[] rectCumulAreas;

    private final Random rng = new Random();

    public Solution(final int[][] rects) {
        this.rects = rects;

        rectCumulAreas = Arrays.stream(rects)
                               .mapToInt(rect -> (rect[2] - rect[0] + 1)
                                                 * (rect[3] - rect[1] + 1))
                               .toArray();
        Arrays.parallelPrefix(rectCumulAreas, Integer::sum);
    }

    public int[] pick() {
        final int totalArea = rectCumulAreas[rectCumulAreas.length - 1];
        final int areaPick = rng.nextInt(totalArea);

        // same as previous solution, but find the rectangle index via binary
        // search rather than linear search
        int lefti = 0, righti = rectCumulAreas.length - 1;

        while (lefti < righti) {
            final int middlei = lefti + (righti - lefti) / 2;

            if (areaPick < rectCumulAreas[middlei]) {
                righti = middlei;
            } else {
                lefti = middlei + 1;
            }
        }

        final var rect = rects[lefti];

        final int x = rect[0] + rng.nextInt(rect[2] + 1 - rect[0]);
        final int y = rect[1] + rng.nextInt(rect[3] + 1 - rect[1]);

        return new int[] {x, y};
    }
}
