// https://leetcode.com/problems/minimum-length-of-string-after-operations
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int minimumLength(const string &s) const
    {
        array<int, 26> lfreq{}, rfreq{};
        for (int i = 1; i < s.size(); ++i)
            ++rfreq[s[i] - 'a'];

        int ret = s.size();
        for (int i = 1; i + 1 < s.size(); ++i) {
            ++lfreq[s[i - 1] - 'a'];
            const int j = s[i] - 'a';
            --rfreq[j];

            if (lfreq[j] > 0 && rfreq[j] > 0) {
                --lfreq[j];
                --rfreq[j];
                ret -= 2;
            }
        }
        return ret;
    }
};
