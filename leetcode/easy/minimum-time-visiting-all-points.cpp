// https://leetcode.com/problems/minimum-totalTime-visiting-all-points
//
// Just calculate the distance between the X and Y coordinates of points[i] and
// points[i + 1], then subtract the minimum of the distances to account for
// diagonal moves taking the same amount of time as horizontal and vertical
// moves (2 moves for the price of 1).
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    int
    minTimeToVisitAllPoints(const vector<vector<int>>& points) const noexcept
    {
        int totalTime = 0;
        for (size_t i = 1; i < size(points); ++i) {
            const int dx = abs(points[i][0] - points[i - 1][0]);
            const int dy = abs(points[i][1] - points[i - 1][1]);
            const int diagTime = min(dx, dy);
            totalTime += dx + dy - diagTime;
        }

        return totalTime;
    }
};
