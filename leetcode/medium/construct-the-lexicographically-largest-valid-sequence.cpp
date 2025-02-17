// https://leetcode.com/problems/construct-the-lexicographically-largest-valid-sequence
// Complexity: runtime O(n^n), space O(n).

class Solution {
public:
    vector<int> constructDistancedSequence(const int n) const
    {
        vector<int> ret(2 * n - 1);
        vector<char> used(n, false); // avoid bool specialization
        const auto find_seq = [&](this const auto &self, const int i) -> bool {
            for (int x = n; x > 0; --x) { // O(n), but max n is small (1..=10)
                if (used[x - 1])
                    continue;
                if (x > 1 && (i + x >= ret.size() || ret[i + x]))
                    continue;

                used[x - 1] = true;
                ret[i] = x;
                if (x > 1)
                    ret[i + x] = x;

                const auto it = find(ret.begin() + i + 1, ret.end(), 0);
                if (it == ret.end() || self(it - ret.begin()))
                    return true;

                // back-track
                if (x > 1)
                    ret[i + x] = 0;
                ret[i] = 0;
                used[x - 1] = false;
            }
            return false;
        };

        const bool found = find_seq(0);
        assert(found);
        return ret;
    }
};
