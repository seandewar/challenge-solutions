// https://leetcode.com/problems/stamping-the-sequence
//
// Same as the Rust version; see that for details.
// Complexity: runtime O(t^2), space O(1) ignoring output.

class Solution {
public:
    vector<int> movesToStamp(const string stamp, string target) const
    {
        const auto isUndone = [](const auto it, const auto endIt) {
            return all_of(it, endIt, [](const char c) { return c == '?'; });
        };
        vector<int> res;

        while (!isUndone(cbegin(target), cend(target))) {
            const auto prevResSize = size(res);

            for (size_t i = 0; i < size(target) - size(stamp) + 1; ++i) {
                auto it = begin(target) + i, endIt = it + size(stamp);
                if (isUndone(it, endIt))
                    continue;

                size_t j = 0;
                for (; j < size(stamp)
                       && (*(it + j) == stamp[j] || *(it + j) == '?');
                     ++j);

                if (j == size(stamp)) {
                    fill(it, endIt, '?');
                    res.push_back(i);
                }
            }

            if (prevResSize == size(res))
                return {};
        }

        reverse(begin(res), end(res));
        return res;
    }
};
