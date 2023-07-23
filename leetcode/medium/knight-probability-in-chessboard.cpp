// https://leetcode.com/problems/knight-probability-in-chessboard
// DFS + Memoization (DP).
// Complexity: runtime O(n^2*k), space O(n^2*k).

class Solution {
  using Mem = array<array<array<double, 100>, 25>, 25>;

  double dfs(int row, int col, int n, int k, Mem &mem) noexcept {
    auto& mem_entry = mem[row][col];
    if (mem_entry[k - 1] > 0.0) {
      return mem_entry[k - 1];
    }
    double prob_sum = 0.0;
    for (const auto &off : {pair{-2, 1},
                            {-1, 2},
                            {1, 2},
                            {2, 1},
                            {2, -1},
                            {1, -2},
                            {-1, -2},
                            {-2, -1}}) {
      const auto& [row_off, col_off] = off;
      const int new_row = row + row_off, new_col = col + col_off;
      if (new_row < 0 || new_row >= n || new_col < 0 || new_col >= n) {
        continue;
      }
      if (k == 1) {
        prob_sum += 1.0;
        continue;
      }
      prob_sum += dfs(new_row, new_col, n, k - 1, mem);
    }
    return mem_entry[k - 1] = prob_sum / 8.0;
  }

public:
  double knightProbability(int n, int k, int start_row, int start_col) {
    if (k == 0) {
      return 1.0; // Constraints have start row/col in-bounds.
    }
    // We don't store invalid moves or the final move in a branch.
    // Therefore we can use 0.0 as a sentinel for an uncached entry.
    Mem mem{};
    return dfs(start_row, start_col, n, k, mem);
  }
};
