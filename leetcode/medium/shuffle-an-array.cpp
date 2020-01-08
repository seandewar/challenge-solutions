// https://leetcode.com/problems/shuffle-an-array/
//
// Standard library algorithm solution.
// Complexity: shuffle runtime O(n), space O(n) [original copy for reset()].

class Solution {
    vector<int> original_;

public:
    Solution(vector<int>& nums) : original_(nums) {}

    /** Resets the array to its original configuration and return it. */
    vector<int> reset()
    {
        return original_;
    }

    /** Returns a random shuffling of the array. */
    vector<int> shuffle()
    {
        // if a random device isn't supported, random_device{}() will return
        // the same sequence. alternatively, seed using the current ticks since
        // the system_clock's epoch as a (less secure) fallback measure instead:
        // chrono::system_clock::now().time_since_epoch().count()
        auto result = original_;
        std::shuffle(begin(result), end(result), mt19937(random_device()()));
        return result;
    }
};

/**
 * Your Solution object will be instantiated and called as such:
 * Solution* obj = new Solution(nums);
 * vector<int> param_1 = obj->reset();
 * vector<int> param_2 = obj->shuffle();
 */

// Alternative Solution: Without std::shuffle(), using the Fisher-Yates
// algorithm.
//
// This algorithm simulates drawing randomly from a hat: elements before the
// current loop index are the 'already drawn' elements, while the others
// are still 'in the hat'. When an element is 'drawn', it is swapped with the
// element at the current loop index, essentially marking it as 'already drawn'
// for the next iteration. This technique allows for in-place shuffling of the
// array without the need for an auxiliary array to represent the 'already
// drawn' elements.
//
// Complexity: runtime O(n), space O(n) [original copy for reset()].
/*
class Solution {
    vector<int> original_;

public:
    Solution(vector<int>& nums) : original_(nums) {}

    vector<int> reset()
    {
        return original_;
    }

    vector<int> shuffle()
    {
        auto result = original_;
        using SizeType = decltype(result)::size_type;

        mt19937 rng(random_device{}());

        for (SizeType i = 0; i < size(result); ++i) {
            uniform_int_distribution<SizeType> dist(i, size(result) - 1);
            swap(result[i], result[dist(rng)]);
        }

        return result;
    }
};
*/
