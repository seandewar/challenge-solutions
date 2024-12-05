// https://leetcode.com/problems/move-pieces-to-obtain-a-string
// Complexity: runtime O(n) [both strings have same size due to constraints],
//             space O(1).

class Solution {
public:
    bool canChange(const string &start, const string &target) const
    {
        for (size_t i = 0, j = 0;; ++i, ++j) {
            i = start.find_first_not_of('_', i);
            j = target.find_first_not_of('_', j);

            if (i >= start.size() || j >= target.size())
                return i >= start.size() && j >= target.size();
            if (start[i] != target[j])
                return false;
            if ((start[i] == 'L' && i < j) || (start[i] == 'R' && i > j))
                return false;
        }
    }
};
