// https://leetcode.com/problems/find-score-of-an-array-after-marking-all-elements
// Complexity: runtime O(nlogn), space O(n).

class Solution {
public:
    long long findScore(vector<int> &nums) const
    {
        vector<size_t> is(nums.size());
        iota(is.begin(), is.end(), 0);

        long long score = 0;
        const auto cmp = [&](const size_t i, const size_t j) {
            const int a = abs(nums[i]), b = abs(nums[j]); // just want the value
            return a > b || (a == b && i > j);
        };
        for (priority_queue q{cmp, std::move(is)}; !q.empty(); q.pop()) {
            const auto top_i = q.top();
            if (nums[top_i] < 0)
                continue; // already marked; skip

            score += nums[top_i];
            nums[top_i] *= -1; // mark
            if (top_i > 0 && nums[top_i - 1] > 0)
                nums[top_i - 1] *= -1; // mark
            if (top_i < nums.size() - 1 && nums[top_i + 1] > 0)
                nums[top_i + 1] *= -1; // mark
        }
        return score;
    }
};
