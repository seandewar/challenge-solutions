// https://leetcode.com/problems/3sum/
//
// See code comments for details.
//
// Complexity: runtime O(n^2*logn), space O(1) [output not considered in
// complexity calculations].

class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) const
    {
        vector<vector<int>> results;

        sort(begin(nums), end(nums));

        // NOTE: we can compare against cend(nums)-2 for this loop & find_if()
        // instead (and cend(nums)-1 for the inner loop & find_if()), provided
        // we check size(nums) > 2 beforehand, but it's not necessary
        for (auto it1 = cbegin(nums); it1 != cend(nums);) {
            const int num1 = *it1;

            // nums is sorted, so if this number > 0, the rest will be too
            if (num1 > 0)
                break;

            for (auto it2 = it1 + 1; it2 != cend(nums);) {
                const int num2 = *it2;

                // again, due to nums being sorted, if the current sum > 0,
                // adding any other number afterwards will also sum > 0
                if (num1 + num2 > 0)
                    break;

                const int needed = -(num1 + num2);

                // nums is sorted, so we can do a binary search O(logn) for the
                // needed number to have our sum == 0
                if (binary_search(it2 + 1, cend(nums), needed))
                    results.push_back({num1, num2, needed});

                // skip past any duplicates of the current number so we don't
                // end up adding repeated triplets to our output
                it2 = find_if(it2 + 1, cend(nums),
                              [=](const int x) { return x != num2; });
            }

            // skip past any duplicates of the current number so we don't end
            // up adding repeated triplets to our output
            it1 = find_if(it1 + 1, cend(nums),
                          [=](const int x) { return x != num1; });
        }

        return results;
    }
};
