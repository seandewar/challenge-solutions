// https://leetcode.com/problems/valid-triangle-number
//
// This uses the triangle inequality theorem; it states that the length of each
// side of a triangle is smaller than the combined lengths of the two other
// sides:
// a + b > c and a + c > b and b + c > a.
//
// Basically, sort the range, then try every a & b. For each a & b, binary
// search for the first c that is not too small, then for the first c that is
// too large, then add the distance between the two to the result.
//
// Not the most efficient solution (b can be found faster), but passes.
//
// Complexity: runtime O((n^2)*logn), space likely O(logn).

class Solution {
public:
    int triangleNumber(vector<int>& nums) const noexcept
    {
        sort(begin(nums), end(nums));
        int res = 0;
        for (auto i = cbegin(nums); i != cend(nums); ++i) {
            for (auto j = i + 1; j != cend(nums); ++j) {
                const int a = *i, b = *j;
                const auto l =
                    partition_point(j + 1, cend(nums), [=](const int c) {
                        return a + c <= b || b + c <= a;
                    });
                const auto r = partition_point(
                    l, cend(nums), [=](const int c) { return a + b > c; });
                res += r - l;
            }
        }
        return res;
    }
};
