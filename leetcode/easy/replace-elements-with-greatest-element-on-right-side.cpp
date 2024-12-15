// https://leetcode.com/problems/replace-elements-with-greatest-element-on-right-side
// Complexity: runtime O(n), space O(1) [input storage reused; O(n) otherwise].

class Solution {
public:
    vector<int> replaceElements(vector<int> &arr) const
    {
        int maxv = -1;
        for (auto it = arr.rbegin(); it != arr.rend(); ++it)
            maxv = max(maxv, exchange(*it, maxv));
        return arr;
    }
};
