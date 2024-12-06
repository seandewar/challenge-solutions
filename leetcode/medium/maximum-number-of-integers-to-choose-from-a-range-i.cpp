// https://leetcode.com/problems/maximum-number-of-integers-to-choose-from-a-range-i
// Complexity: runtime O(n+m), space O(1) [technically O(max possible n)].

class Solution {
public:
    int maxCount(const vector<int> &banned, const int n, const int maxSum) const
    {
        array<bool, 10000> ban_set{};
        for (const int x : banned)
            ban_set[x - 1] = true;

        int ret = 0;
        for (int sum = 0, x = 1; x <= n && sum < maxSum; ++x) {
            if (ban_set[x - 1])
                continue;
            if ((sum += x) <= maxSum)
                ++ret;
        }
        return ret;
    }
};
