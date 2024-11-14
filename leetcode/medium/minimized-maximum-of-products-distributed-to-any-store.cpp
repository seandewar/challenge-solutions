// https://leetcode.com/problems/minimized-maximum-of-products-distributed-to-any-store
//
// Heap-based solution. This was a rather nasty medium; it took a while to get
// the ceiled divisions correct. -_-
//
// Complexity: runtime O((n-m)*logm), space O(m).

class Solution {
public:
    int minimizedMaximum(int n, const vector<int> &quantities) const
    {
        if (quantities.size() == 1)
            return quantities[0];

        struct Entry {
            int max_q, orig_q, n;
        };
        vector<Entry> ents;
        ents.reserve(quantities.size());
        transform(quantities.begin(), quantities.end(), back_inserter(ents),
                  [](const int q) { return Entry{q, q, 1}; });
        priority_queue q{[](const Entry &a, const Entry &b) {
                             return a.max_q < b.max_q
                                    || (a.max_q == b.max_q && a.n < b.n);
                         },
                         std::move(ents)};
        n -= q.size();

        while (n > 0 && q.top().max_q > 1) {
            auto top = q.top();
            q.pop();
            n += top.n;

            const int target_q = q.top().max_q - 1,
                      target_n = target_q > 0
                                     ? (top.orig_q + target_q - 1) // ceil
                                           / target_q
                                     : numeric_limits<int>::max();
            top.n = min(min(n, top.orig_q), target_n);
            top.max_q = (top.orig_q + top.n - 1) / top.n; // ceil
            q.push(top);
            n -= top.n;
        }
        return q.top().max_q;
    }
};
