// https://leetcode.com/problems/maximum-length-of-a-concatenated-string-with-unique-characters
// Complexity: runtime O(n*w), space O(n).

class Solution {
    int dfs(vector<string> &arr, int i, int n, array<bool, 26> used_letters)
    {
        if (i >= static_cast<int>(size(arr)))
            return n;
        if (i >= 0) {
            for (char c : arr[i]) {
                if (exchange(used_letters[c - 'a'], true))
                    return n;
            }
            n += size(arr[i]);
        }

        int maxn = n;
        for (int j = i + 1; j < size(arr); ++j)
            maxn = max(maxn, dfs(arr, j, n, used_letters));
        return maxn;
    }

public:
    int maxLength(vector<string> &arr)
    {
        return dfs(arr, -1, 0, {});
    }
};
