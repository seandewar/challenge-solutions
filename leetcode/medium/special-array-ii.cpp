// https://leetcode.com/problems/special-array-ii
// Complexity: runtime O(n+qlogn), space O(n).

class Solution {
public:
    vector<bool> isArraySpecial(const vector<int> &nums,
                                const vector<vector<int>> &queries)
    {
        vector<pair<int, int>> ivs;
        for (int l = 0; l < nums.size();) {
            int r = l + 1;
            for (; r < nums.size() && ((nums[r - 1] ^ nums[r]) & 1); ++r) {}
            if (r > l + 1)
                ivs.emplace_back(l, r - 1);
            l = r;
        }

        vector<bool> ret;
        ret.reserve(queries.size());
        for (const auto &q : queries) {
            const auto it =
                upper_bound(ivs.begin(), ivs.end(), pair{q[0] + 1, 0});
            ret.push_back(q[0] == q[1]
                          || (it != ivs.begin() && (it - 1)->second >= q[1]));
        }
        return ret;
    }
};
