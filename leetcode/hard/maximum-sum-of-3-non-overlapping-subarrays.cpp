// https://leetcode.com/problems/maximum-sum-of-3-non-overlapping-subarrays
// Complexity: runtime O(n), space O(n).

class Solution {
public:
    vector<int> maxSumOfThreeSubarrays(vector<int> &nums, const int k) const
    {
        int prev =
            exchange(nums[0], accumulate(nums.begin(), nums.begin() + k, 0));
        for (int i = 1; i < nums.size() + 1 - k; ++i)
            prev = exchange(nums[i], nums[i - 1] - prev + nums[i - 1 + k]);
        nums.resize(nums.size() + 1 - k);

        vector<int> max_rs(nums.size() - k - k);
        max_rs.back() = max_rs.size() + k + k - 1;
        for (int i = max_rs.size() - 2; i >= 0; --i) {
            const int r = i + k + k, r_prev = max_rs[i + 1];
            max_rs[i] = nums[r_prev] > nums[r] ? r_prev : r;
        }

        vector<int> max_js(nums.size() - k - k);
        max_js.back() = max_js.size() + k - 1;
        for (int i = max_js.size() - 2; i >= 0; --i) {
            // big chungus
            const int j = i + k, r = max_rs[i], sum = nums[j] + nums[r],
                      j_prev = max_js[i + 1], r_prev = max_rs[j_prev - k],
                      sum_prev = nums[j_prev] + nums[r_prev];
            max_js[i] = sum_prev > sum ? j_prev : j;
        }

        int ret_sum = 0;
        vector<int> ret;
        ret.reserve(3);
        for (int i = 0; i < nums.size() - k - k; ++i) {
            const int j = max_js[i], r = max_rs[j - k],
                      sum = nums[i] + nums[j] + nums[r];
            if (sum > ret_sum) {
                ret = {i, j, r};
                ret_sum = sum;
            }
        }
        return ret;
    }
};
