// https://leetcode.com/problems/remove-sub-folders-from-the-filesystem
// Faster than 100%.
// Complexity: runtime O(n*mlogn), space O(logn) [O(n*m) if not ignoring output]
// [where n == folder.size(), m == max folder[i].size()].

class Solution {
public:
    vector<string> removeSubfolders(vector<string> &folder) const
    {
        sort(folder.begin(), folder.end()); // want parents first
        vector<string> ret;
        for (string_view parent = "/"; const auto &f : folder) {
            if (!f.starts_with(parent) || f[parent.size()] != '/') {
                ret.push_back(f);
                parent = f;
            }
        }
        return ret;
    }
};
