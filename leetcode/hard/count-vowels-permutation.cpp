// https://leetcode.com/problems/count-vowels-permutation
// Dynamic programming (bottom-up).
// Complexity: runtime O(n), space O(n).

class Solution {
  static constexpr int MOD = 1'000'000'007;

public:
  int countVowelPermutation(const int n) const {
    array<vector<int>, 5> map = {vector{1}, vector{0, 2}, vector{0, 1, 3, 4},
                                 vector{2, 4}, vector{0}};
    vector<array<int, 5>> mem(n);
    mem.front().fill(1);
    for (int i = 0; i < size(mem) - 1; ++i) {
      for (int j = 0; j < size(map); ++j) {
        for (const int k : map[j])
          mem[i + 1][k] = (mem[i + 1][k] + mem[i][j]) % MOD;
      }
    }
    int ret = 0;
    for (const int x : mem.back())
      ret = (ret + x) % MOD;
    return ret;
  }
};
