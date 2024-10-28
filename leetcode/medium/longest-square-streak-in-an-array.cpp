// https://leetcode.com/problems/longest-square-streak-in-an-array
// Complexity: runtime O(nlogn), space O(n).

class Solution {
public:
    int longestSquareStreak(vector<int> &nums) const
    {
        sort(nums.begin(), nums.end());
        int ret = -1;
        unordered_map<int, int> sq_map;
        for (const int v : nums) {
            int len = 1;
            if (const auto it = sq_map.find(v); it != sq_map.end())
                len = it->second + 1;
            if (len > 1)
                ret = max(ret, len);
            if (v <= static_cast<int>(sqrt(100'000))) // problem constraint
                sq_map.emplace(v * v, len);
        }
        return ret;
    }
};
