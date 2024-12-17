// https://leetcode.com/problems/construct-string-with-repeat-limit
// Complexity: runtime O(n), space O(1). [only a constant 26 chars to consider]

class Solution {
public:
    string repeatLimitedString(string &s, const int repeatLimit) const
    {
        array<int, 26> freqs{};
        for (const char c : s)
            ++freqs[c - 'a'];

        s.clear();
        const auto cmp = [](const int x) { return x > 0; };
        for (auto it = find_if(freqs.rbegin(), freqs.rend(), cmp),
                  it2 = freqs.rend();
             it != freqs.rend(); it = it2) {
            it2 = find_if(it + 1, freqs.rend(), cmp);

            while (true) {
                const int take = min(repeatLimit, *it);
                *it -= take;
                fill_n(back_inserter(s), take, 'z' - (it - freqs.rbegin()));
                if (*it <= 0 || it2 == freqs.rend())
                    break;

                s.push_back('z' - (it2 - freqs.rbegin()));
                if (--*it2 <= 0)
                    it2 = find_if(it2 + 1, freqs.rend(), cmp);
            }
        }
        return s;
    }
};
