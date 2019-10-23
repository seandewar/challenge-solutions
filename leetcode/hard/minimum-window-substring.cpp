// https://leetcode.com/problems/minimum-window-substring/
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    string minWindow(string s, string t)
    {
        unordered_map<char, string::size_type> winTCharCounts;
        auto winTCompletedUniques = 0u;

        const auto tCharCounts = [&]() { // lambda keeps this immutable
            decltype(winTCharCounts) result;
            for (const auto c : t) ++result[c];
            return result;
        }();

        auto minWinBegin = cbegin(s);
        auto minWinEnd = minWinBegin;

        for (auto winBegin = cbegin(s), winLast = winBegin;
             winLast != cend(s); ++winLast) {
            if (const auto tfit = tCharCounts.find(*winLast);
                tfit != cend(tCharCounts)
                && ++winTCharCounts[*winLast] == tfit->second
                && ++winTCompletedUniques == size(tCharCounts)) {
                // S's window covers all chars in T - try to shrink this window
                // from the front as much as possible while still containing
                // all chars in T.

                for (; winTCompletedUniques == size(tCharCounts); ++winBegin) {
                    if (const auto tfit = tCharCounts.find(*winBegin);
                        tfit != cend(tCharCounts)
                        && winTCharCounts[*winBegin]-- == tfit->second) {
                        --winTCompletedUniques;

                        if (const auto minWinDist = distance(minWinBegin,
                                                             minWinEnd);
                            distance(winBegin, winLast + 1) < minWinDist
                            || minWinDist == 0) {
                            minWinBegin = winBegin;
                            minWinEnd = winLast + 1;
                        }
                    }
                }
            }
        }

        return string(minWinBegin, minWinEnd);
    }
};
