// https://leetcode.com/problems/3sum/
//
// This solution sorts the input and uses 2 iterators to select 2 candidate
// numbers for our sum, then uses a binary search to search for the 3rd number
// needed to make the sum of our numbers == 0; if it succeeds, the triplet is
// added to our output.
//
// This solution avoids outputting duplicate triplets by fast-forwarding each
// iterator past any numbers that are duplicates of the number we just used as a
// candidate. We can do this easily due to the input being sorted.
//
// See code comments for more details.
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

// Alternative Solution: use a hash map to look-up the 3rd number, rather than
// a binary search; faster runtime in exchange for more space.
//
// Complexity: runtime O(n^2), space O(n).
/*
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) const
    {
        vector<vector<int>> results;

        sort(begin(nums), end(nums));

        // because nums is sorted, if there are duplicates of a number in nums,
        // the last occurrence of the number will be the one whose index is
        // saved in the map (earlier indices will be overwritten by the loop)
        unordered_map<int, vector<int>::size_type> numsMap;

        for (vector<int>::size_type i = 0; i < size(nums); ++i)
            numsMap[nums[i]] = i;

        for (auto it1 = cbegin(nums); it1 != cend(nums);) {
            const int num1 = *it1;

            if (num1 > 0)
                break;

            for (auto it2 = it1 + 1; it2 != cend(nums);) {
                const int num2 = *it2;

                if (num1 + num2 > 0)
                    break;

                const int needed = -(num1 + num2);

                // use hash map to lookup the missing number's index, and
                // ensure it exists after this iterator (after num2 in the
                // sequence)
                //
                // because the map stores the index of the last occurrence of a
                // number in nums (important if there are duplicates), we can
                // safely test that the stored index > our current to ensure
                // that at least 1 copy of the missing number exists in the
                // last part of nums that we can use
                //
                // NOTE: the > comparison causes implicit conversion to
                // unsigned, as rhs type (vector::difference_type) is signed
                // (vs lhs size_type used in our map). this is fine, though
                if (const auto fIt = numsMap.find(needed);
                    fIt != cend(numsMap) && fIt->second > it2 - cbegin(nums))
                    results.push_back({num1, num2, needed});

                it2 = find_if(it2 + 1, cend(nums),
                              [=](const int x) { return x != num2; });
            }

            it1 = find_if(it1 + 1, cend(nums),
                          [=](const int x) { return x != num1; });
        }

        return results;
    }
};
*/
