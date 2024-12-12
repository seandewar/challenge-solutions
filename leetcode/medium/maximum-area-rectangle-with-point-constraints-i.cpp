// https://leetcode.com/problems/maximum-area-rectangle-with-point-constraints-i
// Complexity: runtime O(n^2), space O(1).

class Solution {
public:
    int maxRectangleArea(vector<vector<int>> &points) const
    {
        sort(points.begin(), points.end());
        int ret = 0;
        for (int i = 0; i < points.size() - 3; ++i) {
            const auto &botleft = points[i], &topleft = points[i + 1];
            if (botleft[0] != topleft[0])
                continue; // bad top-left; no more candidates

            for (int k = i + 2; k < points.size() - 1; ++k) {
                const auto &botright = points[k], &topright = points[k + 1];
                if (botright[1] > botleft[1] && botright[1] <= topleft[1])
                    break; // bad bot-right and contained; no more candidates
                if (botright[1] != botleft[1])
                    continue; // bad bot-right, but outside; skip
                if (botright[0] != topright[0] || topleft[1] < topright[1])
                    break; // bad top-right; no more candidates
                if (topleft[1] > topright[1])
                    continue; // bad top-right, but valid might be higher; skip

                const int width = botright[0] - botleft[0],
                          height = topleft[1] - botleft[1];
                ret = max(ret, width * height);
                break;
            }
        }
        return ret == 0 ? -1 : ret;
    }
};
