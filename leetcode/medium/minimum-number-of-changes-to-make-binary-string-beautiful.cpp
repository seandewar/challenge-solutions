// https://leetcode.com/problems/minimum-number-of-changes-to-make-binary-string-beautiful
//
// The editorial does better, but this passes. The outer loop finds divisors,
// which only runs the inner O(n) loop ~sqrt(n) times (divisors come in pairs).
//
// Complexity: runtime O(sqrt(n)*n), space O(1).

class Solution {
public:
    int minChanges(const string &s) const
    {
        int ret = numeric_limits<int>::max();
        for (size_t div = 1; div < s.size(); ++div) {
            if (s.size() % div != 0)
                continue;
            const int step = s.size() / div;
            if (step % 2 != 0)
                continue;
            int changes = 0;
            for (size_t i = 0; i < s.size() && changes < ret; i += step) {
                const int zeroes =
                    count(s.begin() + i, s.begin() + i + step, '0');
                changes += min(zeroes, step - zeroes);
            }
            ret = min(ret, changes);
        }
        return ret;
    }
};
