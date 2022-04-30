// https://leetcode.com/problems/sort-characters-by-frequency
// Bucket sort (by frequency count).
// Complexity: runtime O(n), space O(1) ignoring output (we're re-using the
//                                                       input string anyway).

class Solution {
public:
    string frequencySort(string s) const
    {
        constexpr size_t ArraySize = 'z' - '0' + 1;
        array<string::size_type, ArraySize> counts{};
        for (const auto c : s)
            ++counts[c - '0'];

        array<char, ArraySize> letters;
        iota(begin(letters), end(letters), '0');
        sort(begin(letters), end(letters),
             [&](const auto a, const auto b) noexcept {
                 return counts[a - '0'] > counts[b - '0'];
             });

        s.clear();
        for (const auto c : letters) {
            if (counts[c - '0'] == 0)
                break;

            s.append(counts[c - '0'], c);
        }
        return s;
    }
};
