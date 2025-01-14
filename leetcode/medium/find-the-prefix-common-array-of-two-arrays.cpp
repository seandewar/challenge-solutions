// https://leetcode.com/problems/find-the-prefix-common-array-of-two-arrays
// Complexity: runtime O(n), space O(n) [where n == length of permutation]

class Solution {
public:
    vector<int> findThePrefixCommonArray(const vector<int> &a,
                                         const vector<int> &b) const
    {
        array<int, 50> afreqs{}, bfreqs{};
        vector<int> ret;
        ret.reserve(a.size());
        for (int i = 0, count = 0; i < a.size(); ++i) {
            const int ai = a[i] - 1, bi = b[i] - 1;
            if (++afreqs[ai] <= bfreqs[ai])
                ++count;
            if (++bfreqs[bi] <= afreqs[bi])
                ++count;

            ret.push_back(count);
        }
        return ret;
    }
};
