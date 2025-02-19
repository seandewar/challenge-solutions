// https://leetcode.com/problems/construct-smallest-number-from-di-string
// Complexity: runtime O(d^n), space O(d+n).

class Solution {
public:
    string smallestNumber(const string &pattern) const
    {
        string ret, buf;
        buf.reserve(pattern.size() + 1);
        array<bool, 9> used{};
        // index of first char less than ret in buf, or -1.
        // used to keep track of whether buf is lexicographically smaller
        // without re-comparing both strings each time
        int lt_i = -1;

        const auto find_smallest = [&](this const auto &self) {
            const auto i = buf.size();
            if (i > pattern.size()) {
                ret = buf;
                lt_i = -1; // ret and buf now equal
                return;
            }

            const int l = i > 0 && pattern[i - 1] == 'I'
                              ? min(buf.back() - '0' + 1, 9)
                              : 1;
            const int r = i > 0 && pattern[i - 1] == 'D'
                              ? max(buf.back() - '0' - 1, 1)
                              : 9;

            for (int d = l; d <= r; ++d) {
                // reloaded each time, as ret may have changed
                const int ret_d = ret.empty() ? 10 : ret[i] - '0';
                if (lt_i == i && d == ret_d)
                    lt_i = -1;
                if (lt_i < 0 && d > ret_d)
                    break; // buf will be lexicographically bigger than ret; no
                           // more candidates
                if (lt_i < 0 && d < ret_d)
                    lt_i = i;

                if (exchange(used[d - 1], true))
                    continue;

                buf.push_back(d + '0');
                self();

                // back-track
                used[d - 1] = false;
                buf.pop_back();
            }

            if (lt_i == i)
                lt_i = -1;
        };

        find_smallest();
        return ret;
    }
};
