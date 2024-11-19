// https://leetcode.com/problems/maximum-sum-of-distinct-subarrays-with-length-k
// Faster than 100%.
// Complexity: runtime O(n), space O(1) [technically O(max(nums[i]))].

class Solution {
public:
    long long maximumSubarraySum(const vector<int> &nums, const int k) const
    {
        array<int, 100001> freqs{};
        long long sum = 0;
        int dupes = 0;
        for (int i = 0; i < k; ++i) {
            sum += nums[i];
            if (++freqs[nums[i]] == 2)
                ++dupes;
        }

        long long ret = dupes == 0 ? sum : 0;
        for (int i = 0; i < nums.size() - k; ++i) {
            sum -= nums[i];
            sum += nums[i + k];
            if (--freqs[nums[i]] == 1)
                --dupes;
            if (++freqs[nums[i + k]] == 2)
                ++dupes;
            if (dupes == 0)
                ret = max(ret, sum);
        }
        return ret;
    }
};
