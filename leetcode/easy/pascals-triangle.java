// https://leetcode.com/problems/pascals-triangle/
//
// Iterative solution. May also be done recursively if you want.
//
// Complexity: runtime O(n^2), space O(1) [O(n^2) if output is considered, but
// it should be ignored for complexity calculations].

class Solution {
    public List<List<Integer>> generate(final int numRows) {
        final var result = new ArrayList<List<Integer>>(numRows);

        for (int i = 0; i < numRows; ++i) {
            final var row = new ArrayList<Integer>(numRows);

            for (int j = 0; j <= i; ++j) {
                if (j == 0 || j == i) {
                    row.add(1);
                } else {
                    row.add(result.get(i - 1).get(j - 1) +
                            result.get(i - 1).get(j));
                }
            }

            result.add(row);
        }

        return result;
    }
}
