// https://leetcode.com/problems/max-points-on-a-line/
//
// This is a pretty nasty problem. Took me quite a while to create a correct
// solution for this one, however, the method used here isn't difficult to
// grasp:
//
// This solution calculates the gradient between each point on the graph. If the
// gradients between any two points match, they must lie on the same line; this
// property is used to count the max amount of points on the same line, while
// also accounting for duplicate points.
//
// See code comments for more details.
//
// Complexity: runtime O(n^2), space O(n).

namespace std { // injecting custom impl of std::hash for my convenience

template<>
struct hash<pair<int, int>> {
    // with the method of hashing I'm using, we require size of size_t to be at
    // least as large as the size of 2 ints. there are much better ways to hash
    // this, but this is easy and guarantees no hash collisions for all possible
    // pair<int, int>s
    static_assert(sizeof(int) * 2 <= sizeof(size_t));

    size_t operator()(const pair<int, int>& p) const
    {
        return (static_cast<size_t>(p.first) << (sizeof(int) * CHAR_BIT))
               | p.second;
    }
};

} // namespace std

class Solution {
public:
    int maxPoints(const vector<vector<int>>& points) const
    {
        // map to store, for each point, the number of other points with the
        // same gradient, with the gradient as the key
        unordered_map<pair<int, int>, int> slopeCounts;
        int result = 0;

        for (const auto& p : points) {
            int duplicates = 0; // number of q points that are duplicates of p
            slopeCounts.clear();

            for (const auto& q : points) {
                // calculate the gradient between p and q
                const int dy = q[1] - p[1], dx = q[0] - p[0];

                if (dy == 0 && dx == 0) {
                    // this is a duplicate point of p (or this is p itself)
                    ++duplicates;
                    continue;
                }

                // calculate the gcd of dy and dx. we divide dy and dx by this
                // value to ensure that equivalent gradients such as {6,8} and
                // {12,16} are reduced to the same key for hashing (in this
                // case, both examples will be keyed as {3,4})
                const int slopeGcd = gcd(dy, dx);
                ++slopeCounts[{dy / slopeGcd, dx / slopeGcd}];
            }

            const auto maxCountIt = max_element(
                    cbegin(slopeCounts), cend(slopeCounts),
                    [](const auto& a, const auto& b) {
                        return a.second < b.second;
                    });

            const int maxCount = (maxCountIt != cend(slopeCounts))
                                 ? maxCountIt->second
                                 : 0;

            result = max(result, maxCount + duplicates);
        }

        return result;
    }
};
