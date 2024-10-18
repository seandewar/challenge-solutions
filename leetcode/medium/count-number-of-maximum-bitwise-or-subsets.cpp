// https://leetcode.com/problems/count-number-of-maximum-bitwise-or-subsets
// Faster than 99.88%.
// Complexity: runtime O(n*min(n,k)), space O(min(n,k)) [where k is the max OR].

class Solution {
public:
    int countMaxOrSubsets(const vector<int> &nums) const
    {
        unordered_map<int, int> memo; // OR value -> subset count
        memo.reserve(nums.size() + 1);
        memo[0] = 1;
        vector<pair<int, int>> todo; // pending updates to memo, applied after
        todo.reserve(nums.size());   // iterating (as to not mess things up)
        int target = 0;
        for (const int x : nums) {
            target |= x;
            for (const auto &[k, v] : memo)
                todo.emplace_back(k | x, v);
            for (const auto &[k, v] : todo)
                memo[k] += v;
            todo.clear();
        }
        return memo[target];
    }
};
