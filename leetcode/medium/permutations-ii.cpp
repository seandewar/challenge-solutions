// https://leetcode.com/problems/permutations-ii/
//
// Back-tracking with hash map.
// A standard library solution also exists with std::next_permutation().
//
// Complexity: runtime O(n!), space O(n).

class Solution {
    void populatePermutations(vector<vector<int>>& out,
                              unordered_map<int, int>& remainingCounts,
                              const size_t total, vector<int>& perm) const
    {
        if (size(perm) >= total) {
            out.push_back(perm);
            return;
        }

        for (auto& [num, rem] : remainingCounts) {
            if (rem <= 0) {
                continue;
            }

            --rem;
            perm.push_back(num);
            populatePermutations(out, remainingCounts, total, perm);
            perm.pop_back();
            ++rem;
        }
    }

public:
    vector<vector<int>> permuteUnique(const vector<int>& nums) const
    {
        unordered_map<int, int> remainingCounts;
        for (const int x : nums) {
            ++remainingCounts[x];
        }

        vector<vector<int>> result;
        vector<int> perm;
        perm.reserve(size(nums));
        populatePermutations(result, remainingCounts, size(nums), perm);
        return result;
    }
};
