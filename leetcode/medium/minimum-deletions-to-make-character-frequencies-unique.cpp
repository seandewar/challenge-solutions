// https://leetcode.com/problems/minimum-deletions-to-make-character-frequencies-unique
//
// It's possible to do this slightly faster without sorting, but I haven't
// thought about it much.
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int minDeletions(const string s) const noexcept
    {
        array<int, 26> counts{};
        for (const char c : s)
            ++counts[c - 'a'];

        sort(begin(counts), end(counts));
        int deletions = 0;
        for (int i = 1; i < size(counts); ++i) {
            if (counts[i] != counts[i - 1])
                continue;

            int new_freq = max(0, counts[0] - 1);
            for (int j = i; j > 0; --j) {
                if (counts[j] - counts[j - 1] > 1) {
                    new_freq = counts[j] - 1;
                    break;
                }
            }
            deletions += counts[i] - new_freq;
            counts[i] = new_freq;
            sort(begin(counts), begin(counts) + i + 1);
        }

        return deletions;
    }
};
