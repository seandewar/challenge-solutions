// https://leetcode.com/problems/two-sum-ii-input-array-is-sorted
// Complexity: runtime O(nlogn), space O(1).

class Solution {
public:
    vector<int> twoSum(const vector<int> &numbers, const int target) const
    {
        for (auto it = cbegin(numbers); it != cend(numbers);
             it = upper_bound(it, cend(numbers), *it)) {
            const int need = target - *it;
            const auto it2 = lower_bound(next(it), cend(numbers), need);
            if (it2 != cend(numbers) && *it2 == need) {
                return {static_cast<int>(distance(cbegin(numbers), it) + 1),
                        static_cast<int>(distance(cbegin(numbers), it2) + 1)};
            }
        }
        abort();
    }
};
