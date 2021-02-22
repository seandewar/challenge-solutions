// https://leetcode.com/problems/h-index/
//
// Complexity: runtime O(n*logn), likely space O(1) or O(logn), depending on
// sort() impl.

class Solution {
public:
    int hIndex(vector<int>& citations) const noexcept
    {
        sort(begin(citations), end(citations));

        // a binary search can be done here instead, but this doesn't change the
        // worst-case runtime complexity
        for (size_t i = 0; i < size(citations); ++i) {
            if (const auto h = size(citations) - i; h <= citations[i]) {
                return h;
            }
        }

        return 0;
    }
};
