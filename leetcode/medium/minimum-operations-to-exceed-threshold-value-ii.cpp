// https://leetcode.com/problems/minimum-operations-to-exceed-threshold-value-ii
// Complexity: runtime O(nlogn), space O(n).

class Solution {
public:
    int minOperations(const vector<int> &nums, const int k) const
    {
        vector<long long> qbuf;
        qbuf.reserve(nums.size());
        ranges::copy(nums, back_inserter(qbuf));

        priority_queue q(greater{}, std::move(qbuf));
        int ret = 0;
        for (; q.size() > 1 && q.top() < k; ++ret) {
            const auto a = q.top();
            q.pop();
            const auto b = q.top();
            q.pop();
            q.push(2 * a + b);
        }
        return ret;
    }
};
