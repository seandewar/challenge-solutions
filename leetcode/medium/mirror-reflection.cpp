// https://leetcode.com/problems/mirror-reflection
//
// Due to how reflection works, we can just mirror the room rather than the beam
// after hitting a wall, which is pretty easy to reason about.

class Solution {
public:
    int mirrorReflection(const int p, const int q) const noexcept
    {
        bool mirrorx = false, mirrory = false;
        for (int y = q; y != p; y += q, mirrorx = !mirrorx) {
            if (y > p) {
                mirrory = !mirrory;
                y -= p;
            }
        }
        if (mirrorx && !mirrory)
            return 2;
        if (!mirrorx && mirrory)
            return 0;
        // we either hit #1 directly, or hit the starting corner, which will
        // cause us to hit #1 anyway on the next bounce
        return 1;
    }
};
