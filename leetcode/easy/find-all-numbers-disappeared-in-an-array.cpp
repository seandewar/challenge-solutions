// https://leetcode.com/problems/find-all-numbers-disappeared-in-an-array
//
// This solution meets the follow-up's criteria of also running in linear time
// with no extra space except for the output (though we actually reuse the
// input's storage here).
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    vector<int> findDisappearedNumbers(vector<int> &nums) const
    {
        for (int i = 0; i < nums.size();) {
            const int want_i = nums[i] - 1;
            if (i != want_i && nums[i] != nums[want_i])
                swap(nums[i], nums[want_i]);
            else
                ++i;
        }

        int j = 0;
        for (int i = 0; i < nums.size(); ++i)
            if (nums[i] - 1 != i)
                nums[j++] = i + 1;
        nums.resize(j);
        return nums;
    }
};
