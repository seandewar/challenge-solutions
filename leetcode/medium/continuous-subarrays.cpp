// https://leetcode.com/problems/continuous-subarrays
// Complexity: runtime O(nlogn), space O(n).

class Solution {
public:
    long long continuousSubarrays(const vector<int> &nums) const
    {
        priority_queue<int> maxq{{}, {nums[0]}}, pop_maxq;
        priority_queue<int, vector<int>, greater<int>> minq{{}, {nums[0]}},
            pop_minq;
        long long ret = 1;
        for (int l = 0, r = 0; r < nums.size() - 1;) {
            if (r < nums.size() - 1 && maxq.top() - minq.top() <= 2) {
                maxq.push(nums[++r]);
                minq.push(nums[r]);
            }
            while (maxq.top() - minq.top() > 2) { // always true if l == r
                pop_maxq.push(nums[l]);
                pop_minq.push(nums[l++]);
                while (!pop_maxq.empty() && pop_maxq.top() == maxq.top()) {
                    pop_maxq.pop();
                    maxq.pop();
                }
                while (!pop_minq.empty() && pop_minq.top() == minq.top()) {
                    pop_minq.pop();
                    minq.pop();
                }
            }
            ret += 1 + r - l;
        }
        return ret;
    }
};
