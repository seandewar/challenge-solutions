// https://leetcode.com/problems/two-best-non-overlapping-events
// Complexity: runtime O(nlogn), space O(n).

class Solution {
public:
    int maxTwoEvents(vector<vector<int>> &events) const
    {
        sort(events.begin(), events.end(),
             [](const auto &a, const auto &b) { return a[1] < b[1]; });
        vector<size_t> starts_asc(events.size());
        iota(starts_asc.begin(), starts_asc.end(), 0);
        sort(starts_asc.begin(), starts_asc.end(),
             [&](const size_t i, const size_t j) {
                 return events[i][0] < events[j][0];
             });

        vector<int> values;
        values.reserve(events.size());
        transform(events.begin(), events.end(), back_inserter(values),
                  [](const auto &e) { return e[2]; });
        priority_queue<int> q{{}, std::move(values)}, popq;
        int ret = 0;
        for (size_t popi = 0; const auto &e : events) {
            for (; popi < events.size() && events[starts_asc[popi]][0] <= e[1];
                 ++popi)
                popq.push(events[starts_asc[popi]][2]);

            while (!popq.empty() && q.top() == popq.top()) {
                q.pop();
                popq.pop();
            }
            ret = max(ret, (q.empty() ? 0 : q.top()) + e[2]);
        }
        return ret;
    }
};
