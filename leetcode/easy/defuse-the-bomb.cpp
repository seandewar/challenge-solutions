// https://leetcode.com/problems/defuse-the-bomb
// Complexity: runtime O(n), space O(n) including output, O(1) otherwise.

class Solution {
public:
    vector<int> decrypt(const vector<int> &code, const int k) const
    {
        vector<int> ret(code.size());
        if (k == 0)
            return ret;

        const auto idx = [&](const int i) {
            return (code.size() + i) % code.size();
        };

        int sum = 0;
        for (int i = 1; i <= abs(k); ++i)
            sum += code[idx(k > 0 ? i : -i)];

        for (int i = 0; i < code.size(); ++i) {
            ret[i] = sum;
            sum -= code[idx(i + (k > 0 ? 1 : k))];
            sum += code[idx(i + (k > 0 ? 1 + k : 0))];
        }
        return ret;
    }
};
