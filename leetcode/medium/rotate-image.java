// https://leetcode.com/problems/rotate-image/
//
// The question requires you to rotate the matrix in-place.
//
// To understand this answer, we need to divide the matrix into layers:
//
//   0 0 0 0 0
//   0 1 1 1 0
//   0 1 2 1 0
//   0 1 1 1 0
//   0 0 0 0 0
//
// ...and rotate them individually 90 degrees clockwise.
//
// This solution does so by caching the value at a particular position on a
// layer, and caching the values at the corresponding positions on each side of
// the layer when rotated 0, 90, 180 and 270 degrees clockwise; for example,
// when rotating the element marked "a" on the below layer, we also cache the
// values b, c and d:
//
// - a - -
// -     b
// d     -
// - - c -
//
// ...so that we can rotate the values without worrying about overwriting the
// original values during the rotation and accidentally writing the wrong
// values (however, there is a solution which exists that can swap the values
// in such a way where this isn't a problem).
//
// Complexity: runtime O(n), space O(1).

class Solution {
    public void rotate(final int[][] matrix) {
        final int n = matrix.length;

        for (int layer = 0; layer < n / 2; ++layer) {
            for (int layerTopX = layer;
                 layerTopX < n - layer - 1;
                 ++layerTopX) {
                // values at the corresponding positions on each side of this
                // layer when rotated 0, 90, 180 and 270 degrees clockwise
                final int[] sides = {
                    matrix[layer][layerTopX],
                    matrix[layerTopX][n - 1 - layer],
                    matrix[n - 1 - layer][n - 1 - layerTopX],
                    matrix[n - 1 - layerTopX][layer]
                };

                matrix[layer][layerTopX] = sides[3];
                matrix[layerTopX][n - 1 - layer] = sides[0];
                matrix[n - 1 - layer][n - 1 - layerTopX] = sides[1];
                matrix[n - 1 - layerTopX][layer] = sides[2];
            }
        }
    }
}

// Alternative Solution: swapping.
//
// Same as the above solution, except we swap the layer side values in such a
// way where caching all of the original values at once isn't necessary.
//
// Given a layer:
//
// - a - -
// -     b
// d     -
// - - c -
//
// ...it should look like this after a 90 degree clockwise rotation:
//
// - d - -
// -     a
// c     -
// - - b -
//
// ...to achieve this, we start by swapping a with b; a will now hold the
// original value of b, so we swap a (which now holds the old b value) with c,
// then swap a with d. That's it!
//
// Complexity: runtime O(n), space O(1).
/*
class Solution {
    public void rotate(final int[][] matrix) {
        final int n = matrix.length;

        for (int layer = 0; layer < n / 2; ++layer) {
            for (int layerTopX = layer;
                 layerTopX < n - layer - 1;
                 ++layerTopX) {
                swap(matrix, layerTopX, layer, n - 1 - layer, layerTopX);
                swap(matrix, layerTopX, layer,
                     n - 1 - layerTopX, n - 1 - layer);
                swap(matrix, layerTopX, layer, layer, n - 1 - layerTopX);
            }
        }
    }

    private void swap(final int[][] matrix,
                      final int x1, final int y1,
                      final int x2, final int y2) {
        final int temp = matrix[y2][x2];

        matrix[y2][x2] = matrix[y1][x1];
        matrix[y1][x1] = temp;
    }
}
*/
