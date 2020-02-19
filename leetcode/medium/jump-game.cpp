// https://leetcode.com/problems/jump-game/
//
// Greedy approach.
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    bool canJump(const vector<int>& nums) const
    {
        // holds the index of the current position in nums that we've determined
        // is able to reach the end
        int prevIdx = size(nums) - 1;

        for (int i = size(nums) - 1; i >= 0; --i) {
            // if we are able to reach or surpass prevIdx from here, then there
            // exists a route from here to the end, and we can make i our new
            // prevIdx
            if (i + nums[i] >= prevIdx)
                prevIdx = i;
        }

        return prevIdx == 0; // was the end reachable from the start?
    }
};

// Alternative Solution: Iterative (bottom-up) with dynamic programming.
//
// Complexity: runtime O(n^2), space O(n).
/*
class Solution {
public:
    bool canJump(const vector<int>& nums) const
    {
        // cache stores true values if we can confirm the same index in nums is
        // reachable from index 0; false otherwise
        //
        // NOTE: vector<char> avoids the horrible mistake that is the standard
        // library's explicit specialization of vector<bool>
        vector<char> cache(size(nums));
        cache.front() = true;

        for (int i = 0; i < size(nums) - 1; ++i) {
            if (!cache[i])
                continue; // confirmed unreachable, skip

            const int maxJump = min<int>(nums[i], size(cache) - i - 1);
            fill_n(begin(cache) + i, maxJump + 1, true);
        }

        return cache.back(); // was the last index reachable from the front?
    }
};
*/
