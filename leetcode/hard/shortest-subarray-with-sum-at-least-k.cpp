// https://leetcode.com/problems/shortest-subarray-with-sum-at-least-k
// Complexity: runtime O(nlogn), space O(n).

class Solution {
public:
    int shortestSubarray(const vector<int> &nums, const int k) const
    {
        vector<pair<long long, int>> st{{0, -1}};
        long long pre = 0;
        int ret = numeric_limits<int>::max();
        for (int i = 0; i < nums.size(); ++i) {
            pre += nums[i];
            if (const auto it =
                    lower_bound(st.begin(), st.end(),
                                pair{pre - k, numeric_limits<int>::max()});
                it != st.begin())
                ret = min(ret, i - (it - 1)->second);

            const auto it = lower_bound(st.begin(), st.end(), pair{pre, -1});
            st.resize(it - st.begin());
            st.emplace_back(pre, i);
        }
        return ret == numeric_limits<int>::max() ? -1 : ret;
    }
}
