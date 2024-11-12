// https://leetcode.com/problems/most-beautiful-item-for-each-query
//
// Faster than 100%.
//
// Complexity: runtime O((i+q)*logi), space O(logi) [O(logi + q) if including
// output; where i == size(items), q == size(queries)].

class Solution {
public:
    vector<int> maximumBeauty(vector<vector<int>> &items,
                              const vector<int> &queries) const
    {
        sort(items.begin(), items.end(),
             [](const auto &a, const auto &b) { return a[0] < b[0]; });
        size_t j = 0;
        for (size_t i = 1; i < items.size(); ++i) {
            const int price = items[i][0], beauty = items[i][1];
            if (price > items[j][0]) {
                items[++j][0] = price;
                items[j][1] = items[j - 1][1];
            }
            items[j][1] = max(beauty, items[j][1]);
        }
        // Slightly faster to not call items.resize(j + 1) here and just use
        // items.begin() + j + 1 as the end iterator for our binary searches
        // below, as that skips calling vector destructors. In practice, runtime
        // seems slightly faster if we instead create a new vector of
        // non-allocating price + max beauty pairs rather than reusing the items
        // vector for that purpose, but that increases the space complexity
        // (which doesn't really matter, but :-D).

        vector<int> ret;
        ret.reserve(queries.size());
        for (const int q : queries) {
            const auto it = upper_bound(
                items.begin(), items.begin() + j + 1, q,
                [](const int q, const auto &v) { return q < v[0]; });
            ret.push_back(it != items.begin() ? (*(it - 1))[1] : 0);
        }
        return ret;
    }
};
