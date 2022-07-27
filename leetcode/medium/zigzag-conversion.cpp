// https://leetcode.com/problems/zigzag-conversion
//
// Sorts; another alternative is to plot a matrix and traverse the rows to
// create the output.
//
// Complexity: runtime O(nlogn), space O(n).

class Solution {
public:
    string convert(string s, const int numRows) const
    {
        vector<tuple<char, int, int>> infos;
        infos.reserve(size(s));
        int x = 0, y = 0;
        bool zag = false;
        for (const char c : s) {
            infos.emplace_back(c, x, y);
            if (!zag && y == numRows - 1)
                zag = true;
            if (zag && y == 0)
                zag = false;

            if (zag) {
                --y;
                ++x;
            } else {
                ++y;
            }
        }
        sort(begin(infos), end(infos), [](const auto a, const auto b) {
            return get<2>(a) < get<2>(b)
                   || (get<2>(a) == get<2>(b) && get<1>(a) < get<1>(b));
        });

        s.clear();
        transform(cbegin(infos), cend(infos), back_inserter(s),
                  [](const auto a) { return get<0>(a); });
        return s;
    }
};
