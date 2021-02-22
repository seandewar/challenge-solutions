// https://leetcode.com/problems/permutations/
//
// Uses back-tracking and builds permutations within a vector before appending
// it to our output once the permutation is fully built. Also uses a hash set
// for determining what values in the input have been used already for the
// permutation that we're building (for this reason, it will not work properly
// if the input contains duplicates; this question guarantees the test inputs
// will not have such inputs).
//
// It is possible to do this without needing a hash set and auxillary vector for
// building permuatations, although this approach is probably the more initially
// obvious one; see the alternative solutions below.
//
// Complexity: runtime O(n!), space O(n!) [output not considered].

class Solution {
    void permute(const vector<int>& nums, vector<vector<int>>& out,
                 vector<int>& picks, unordered_set<int>& picksSet) const
    {
        if (size(picks) == size(nums)) {
            out.push_back(picks);
            return;
        }

        for (const int v : nums) {
            if (picksSet.count(v) > 0)
                continue;

            picks.push_back(v);
            picksSet.insert(v);

            permute(nums, out, picks, picksSet);

            // back-track
            picksSet.erase(v);
            picks.pop_back();
        }
    }

public:
    vector<vector<int>> permute(const vector<int>& nums) const
    {
        vector<vector<int>> results;

        vector<int> picks;
        picks.reserve(size(nums));

        unordered_set<int> picksSet;
        picksSet.reserve(size(nums));

        permute(nums, results, picks, picksSet);
        return results;
    }
};

// Alternative Solution: using std::next_permutation().
// Complexity: runtime O(n!), space is likely O(1) or O(logn), depending on
// sort() [input/output not considered].
/*
class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) const
    {
        vector<vector<int>> results;
        sort(begin(nums), end(nums)); // requires sorted nums beforehand

        do {
            results.push_back(nums);
        } while (next_permutation(begin(nums), end(nums)));

        return results;
    }
};
*/

// Alternative Solution: back-tracking, but no hash set or auxillary vectors for
// permutation building.
//
// This solution instead logically removes values from nums to build our
// permutations. This is done by dividing our input array into 2 logical
// sections: the 1st section stores our picked nums for the permutation that
// we're building, while the 2nd section contains the values in our input that
// we haven't picked yet. To pick a number for the permutation being built, we
// simply swap the number from the input's 2nd logical section to the end of its
// 1st, increasing the 1st section's size as we do so (and equivalently,
// decreasing the 2nd section's size). After each recursive call completes, we
// back-track by performing the swap again (and undo the changes to the logical
// sizes, which is implicitly done via the recursive call finishing).
//
// Complexity: runtime O(n!), space O(n!) [output not considered].
/*
class Solution {
    void permute(vector<int>& nums, vector<vector<int>>& out,
                 const vector<int>::size_type picksSize = 0) const
    {
        if (picksSize == size(nums)) {
            out.push_back(nums);
            return;
        }

        for (auto i = picksSize; i < size(nums); ++i) {
            swap(nums[i], nums[picksSize]);
            permute(nums, out, picksSize + 1);
            swap(nums[i], nums[picksSize]); // back-track
        }
    }

public:
    vector<vector<int>> permute(vector<int>& nums) const
    {
        vector<vector<int>> results;
        permute(nums, results);
        return results;
    }
};
*/

// Alternative Solution: no back-tracking.
// Complexity: runtime O(n!*n), space O(n!*n) [output not considered].
/*
class Solution {
    void permute(const vector<int> nums, vector<vector<int>>& out,
                 const vector<int> picks = {}) const
    {
        if (empty(nums)) {
            out.push_back(move(picks));
            return;
        }

        for (vector<int>::size_type i = 0; i < size(nums); ++i) {
            auto newNums = nums, newPicks = picks;
            newPicks.push_back(nums[i]);
            newNums.erase(cbegin(newNums) + i);

            permute(newNums, out, newPicks);
        }
    }

public:
    vector<vector<int>> permute(const vector<int>& nums) const
    {
        vector<vector<int>> results;
        permute(nums, results);
        return results;
    }
};
*/
