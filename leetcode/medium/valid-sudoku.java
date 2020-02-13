// https://leetcode.com/problems/valid-sudoku/
//
// Checks all rows, then columns, then 3x3 squares for duplicated numbers
// individually.
//
// Probably the most intuitive solution. Alternatives include replacing hash
// set with a look-up array of booleans or bitsets, and doing only 1 pass
// through the board in exchange for more space usage. These alternatives have
// the same runtime & space complexity, though.
//
// Complexity: runtime O(1), space O(1). [input always 9x9 board, and this
// solution assumes 9x9; O(n^2) runtime & O(n) space in general, though]

class Solution {
    public boolean isValidSudoku(final char[][] board) {
        final var seen = new HashSet<Character>(9);

        // check rows for duplication
        for (final var row : board) {
            for (final var c : row) {
                if (seenBefore(seen, c))
                    return false;
            }

            seen.clear();
        }

        // check cols for duplication
        for (int x = 0; x < 9; ++x) {
            for (int y = 0; y < 9; ++y) {
                if (seenBefore(seen, board[y][x]))
                    return false;
            }

            seen.clear();
        }

        // check 3x3 squares for duplication
        for (int squareIdx = 0; squareIdx < 9; ++squareIdx) {
            for (int squareY = 0; squareY < 3; ++squareY) {
                for (int squareX = 0; squareX < 3; ++squareX) {
                    final int y = squareY + 3 * (squareIdx / 3);
                    final int x = squareX + 3 * (squareIdx % 3);

                    if (seenBefore(seen, board[y][x]))
                        return false;
                }
            }

            seen.clear();
        }

        return true;
    }

    private boolean seenBefore(final Set<Character> seen, final char c) {
        return c != '.' && !seen.add(c);
    }
}
