// https://leetcode.com/problems/kth-smallest-element-in-a-sorted-matrix
// Complexity: runtime O(n + k*logn), space O(n).

class Solution {
public:
    int kthSmallest(const vector<vector<int>> &matrix, const int k) const
    {
        auto cmp = [&](auto a, auto b) {
            return matrix[a.first][a.second] > matrix[b.first][b.second];
        };
        vector<pair<int, int>> init;
        init.reserve(size(matrix));
        for (int i = 0; i < size(matrix); ++i)
            init.emplace_back(i, 0);
        priority_queue<pair<int, int>, decltype(init), decltype(cmp)> heap(
            cmp, move(init));

        pair<int, int> res;
        for (int i = 0; i < k; ++i) {
            res = heap.top();
            heap.pop();
            if (res.second + 1 < size(matrix[res.first]))
                heap.emplace(res.first, res.second + 1);
        }
        return matrix[res.first][res.second];
    }
};
