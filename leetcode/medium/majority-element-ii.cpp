// https://leetcode.com/problems/majority-element-ii
//
// Using a hash table.
// Complexity: runtime O(n), space O(n).

class Solution {
public:
    vector<int> majorityElement(const vector<int>& nums) const
    {
        unordered_map<int, size_t> counts;
        for (const auto x : nums)
            ++counts[x];

        vector<int> result;
        result.reserve(2);
        for (const auto& [k, v] : counts) {
            if (v > size(nums) / 3)
                result.push_back(k);
        }

        return result;
    }
};

// Alternative Solution: Boyer-Moore majority vote algorithm.
//
// Generally, this algorithm is used with a single candidate value for finding
// the element that appears > n/2 times. However, we can modify it for our > n/3
// case by keeping 2 candidates, as there can only be at most 2 such elements
// that exist in the array.
//
// Note that we will need to check that our candidates are valid after running
// the algorithm, however, as just like the original variant of the algorithm,
// the output is garbage if no majority exists (or if only 1 exists, then the
// 2nd candidate will be garbage).
//
// See my solution for the "Majority Element I" problem for an explanation of
// the original variant of the algorithm.
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    vector<int> majorityElement(const vector<int>& nums) const
    {
        // initial majority values don't matter, but they need to be different
        array<int, 2> maybeMajors{0, -1};
        array<size_t, 2> counts{0, 0};

        for (const int x : nums) {
            if (x == maybeMajors[0])
                ++counts[0];
            else if (x == maybeMajors[1])
                ++counts[1];
            else if (counts[0] == 0) {
                maybeMajors[0] = x;
                counts[0] = 1;
            } else if (counts[1] == 0) {
                maybeMajors[1] = x;
                counts[1] = 1;
            } else {
                --counts[0];
                --counts[1];
            }
        }

        vector<int> result;
        result.reserve(2);
        copy_if(cbegin(maybeMajors), cend(maybeMajors), back_inserter(result),
                [&](const int x) noexcept {
                    return count(cbegin(nums), cend(nums), x) > size(nums) / 3;
                });

        return result;
    }
};
