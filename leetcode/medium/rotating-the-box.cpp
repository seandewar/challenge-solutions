// https://leetcode.com/problems/rotating-the-box
// Complexity: runtime O(n*m), space O(1) [O(n*m) including output].

class Solution {
public:
    vector<vector<char>> rotateTheBox(const vector<vector<char>> &box) const
    {
        vector ret(box[0].size(), vector<char>(box.size()));
        for (int y = 0; y < box.size(); ++y)
            for (int x = 0; x < box[0].size(); ++x)
                ret[x][ret[0].size() - 1 - y] = box[y][x];

        for (int x = 0; x < ret[0].size(); ++x) {
            for (int y = ret.size() - 1, wall_y = ret.size(); y >= 0; --y) {
                char &c = ret[y][x];
                if (c != '#') {
                    if (c == '*')
                        wall_y = y;
                    continue;
                }
                c = '.';
                ret[--wall_y][x] = '#';
            }
        }
        return ret;
    }
};
