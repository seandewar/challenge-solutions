// https://leetcode.com/problems/maximum-beauty-of-an-array-after-applying-operation
//
// Works by treating each number as an interval [x - k, x + k], then finds the
// maximum number of intersections.
//
// Complexity: runtime O(nlogn), space O(n).

class Solution {
public:
    int maximumBeauty(vector<int> &nums, const int k) const
    {
        sort(nums.begin(), nums.end());
        priority_queue<int, vector<int>, greater<int>> q;
        int ret = 0;
        for (const int x : nums) {
            for (; !q.empty() && q.top() + k < x - k; q.pop()) {}
            q.push(x);
            ret = max<int>(ret, q.size());
        }
        return ret;
    }
};
