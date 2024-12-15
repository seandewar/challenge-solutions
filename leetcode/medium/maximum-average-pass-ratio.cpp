// https://leetcode.com/problems/maximum-average-pass-ratio
// Complexity: runtime O(n+elogn), space O(n) [where e == extraStudents].

class Solution {
public:
    double maxAverageRatio(vector<vector<int>> &classes,
                           int extraStudents) const
    {
        vector<size_t> is(classes.size());
        iota(is.begin(), is.end(), 0);
        const auto minGain = [](const auto &v) {
            return static_cast<double>(v[0] + 1) / (v[1] + 1)
                   - static_cast<double>(v[0]) / v[1];
        };

        priority_queue q{[&](const auto i, const auto j) {
                             return minGain(classes[i]) < minGain(classes[j]);
                         },
                         std::move(is)};
        if (classes[q.top()][0] != classes[q.top()][1]) {
            while (extraStudents > 0) {
                const auto topI = q.top();
                auto &v = classes[topI];
                q.pop();

                // Maybe a bit faster than assigning one-by-one; instead, assign
                // as many students as we can without surpassing the minimum
                // ratio gain from instead assigning one student to the next
                // best class.
                const auto nextGain =
                    q.empty() ? 0.0 : minGain(classes[q.top()]);
                // Derived from solving for incr: (capped at extraStudents)
                // (v[0] + incr) / (v[1] + incr) - v[0] / v[1] < nextGain
                const int incr =
                    min<int>(extraStudents,
                             1
                                 + (nextGain * v[1])
                                       / (1 - nextGain
                                          - static_cast<double>(v[0]) / v[1]));
                v[0] += incr;
                v[1] += incr;
                extraStudents -= incr;

                q.push(topI);
            }
        }

        return accumulate(classes.begin(), classes.end(), 0.0,
                          [](const auto acc, const auto &v) {
                              return acc + static_cast<double>(v[0]) / v[1];
                          })
               / classes.size();
    }
};
