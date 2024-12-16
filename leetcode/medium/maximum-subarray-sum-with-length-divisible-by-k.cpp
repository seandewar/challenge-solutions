// https://leetcode.com/problems/maximum-subarray-sum-with-length-divisible-by-k
// Complexity: runtime O(n+k), space O(n+k).

class Solution {
public:
    long long maxSubarraySum(const vector<int> &nums, const int k) const
    {
        vector<long long> prefixes;
        prefixes.reserve(nums.size());
        for (auto sum = 0ll; const int x : nums)
            prefixes.push_back(sum += x);

        auto ret = numeric_limits<long long>::min();
        vector minPrefixes(k, numeric_limits<long long>::max());
        minPrefixes[k - 1] = 0;
        for (size_t i = 0; i < prefixes.size(); ++i) {
            if (i >= k - 1)
                ret = max(ret, prefixes[i] - minPrefixes[i % k]);
            minPrefixes[i % k] = min(minPrefixes[i % k], prefixes[i]);
        }
        return ret;
    }
};
