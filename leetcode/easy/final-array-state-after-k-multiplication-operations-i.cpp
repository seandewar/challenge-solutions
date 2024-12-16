// https://leetcode.com/problems/final-array-state-after-k-multiplication-operations-i
// Complexity: runtime O(klogn), space O(n).

class Solution {
public:
    vector<int> getFinalState(vector<int> &nums, int k,
                              const int multiplier) const
    {
        vector<size_t> is(nums.size());
        iota(is.begin(), is.end(), 0);
        priority_queue q{[&](const size_t i, const size_t j) {
                             return tie(nums[i], i) > tie(nums[j], j);
                         },
                         std::move(is)};
        for (; k > 0; --k) {
            const auto topi = q.top();
            q.pop();
            nums[topi] *= multiplier;
            q.push(topi);
        }
        return nums;
    }
};
