// https://leetcode.com/problems/minimum-number-of-operations-to-move-all-balls-to-each-box
// Complexity: runtime O(n), space O(1) [ignoring output storage].

class Solution {
public:
    vector<int> minOperations(const string &boxes) const
    {
        vector<int> ret;
        ret.reserve(boxes.size());
        ret.push_back(0);
        int r = 0;
        for (int i = 1; i < boxes.size(); ++i) {
            if (boxes[i] == '1') {
                ++r;
                ret.back() += i;
            }
        }
        for (int i = 1, l = 0; i < boxes.size(); ++i) {
            l += boxes[i - 1] == '1';
            ret.push_back(ret.back() - r + l);
            r -= boxes[i] == '1';
        }
        return ret;
    }
};
