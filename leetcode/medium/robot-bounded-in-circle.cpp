// https://leetcode.com/problems/robot-bounded-in-circle
//
// As we always walk using the same instructions, if we end up in the same
// position as we have at some point previously, we're bounded.
// There's only 4 possible directions, so we need only test 4 times.
//
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    bool isRobotBounded(const string &instructions) const
    {
        constexpr array<pair<int, int>, 4> dirs{
            pair{0, -1},
            pair{1, 0},
            pair{0, 1},
            pair{-1, 0},
        };
        set<pair<int, int>> seen{{0, 0}};
        for (int x = 0, y = 0, d = 0, i = 0; i < 4; ++i) {
            for (char c : instructions) {
                if (c == 'G') {
                    x += dirs[d].first;
                    y += dirs[d].second;
                    continue;
                }
                d = c == 'L' ? d - 1 : d + 1;
                d &= 3;
            }
            if (!seen.emplace(x, y).second) {
                return true;
            }
        }
        return false;
    }
};
