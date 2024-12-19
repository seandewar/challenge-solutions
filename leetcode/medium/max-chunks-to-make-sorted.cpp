// https://leetcode.com/problems/max-chunks-to-make-sorted
// Complexity: runtime O(n), space O(n).

class Solution {
public:
    int maxChunksToSorted(const vector<int> &arr) const
    {
        vector<int> min_stack;
        min_stack.reserve(arr.size() - 1);
        min_stack.push_back(arr.back());
        for (int i = arr.size() - 2; i > 0; --i)
            min_stack.push_back(min(arr[i], min_stack.back()));

        int ret = 0;
        for (int max_x = -1; const int x : arr) {
            max_x = max(max_x, x);
            if (min_stack.empty() || min_stack.back() >= max_x)
                ++ret;
            if (!min_stack.empty())
                min_stack.pop_back();
        }
        return ret;
    }
};
