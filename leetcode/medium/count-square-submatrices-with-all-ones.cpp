// https://leetcode.com/problems/count-square-submatrices-with-all-ones
// Complexity: runtime O(n*m), space O(1) [reuses input storage].

class Solution {
public:
    int countSquares(vector<vector<int>> &matrix) const
    {
        int ret = 0;
        for (int i = 0; i < matrix.size(); ++i) {
            for (int j = 0; j < matrix[0].size(); ++j) {
                if (matrix[i][j] > 0 && i > 0 && j > 0) {
                    const int a = matrix[i - 1][j - 1], b = matrix[i - 1][j],
                              c = matrix[i][j - 1];
                    matrix[i][j] = 1 + min(min(a, b), c);
                }
                ret += matrix[i][j];
            }
        }
        return ret;
    }
};
