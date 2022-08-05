// https://leetcode.com/problems/combination-sum-ii
// Good ol' backtrackin'
// Complexity: runtime O(2^n), space O(n) excluding output.

class Solution {
    void dfs(vector<vector<int>> &ret, vector<int> &combo,
             const vector<int> &candidates, const int target, const int i) const
    {
        if (target == 0) {
            ret.push_back(combo);
            return;
        }
        const auto beginIt = cbegin(candidates) + i + 1;
        const auto endIt = upper_bound(beginIt, cend(candidates), target);
        for (auto it = beginIt; it != endIt;) {
            if (it > beginIt && *it == *(it - 1)) {
                it = upper_bound(it, endIt, *it);
                continue;
            }
            combo.push_back(*it);
            dfs(ret, combo, candidates, target - *it, it - cbegin(candidates));
            combo.pop_back();
            ++it;
        }
    }

public:
    vector<vector<int>> combinationSum2(vector<int> &candidates,
                                        const int target) const
    {
        sort(begin(candidates), end(candidates));
        vector<vector<int>> ret;
        vector<int> combo;
        dfs(ret, combo, candidates, target, -1);
        return ret;
    }
};
