// https://leetcode.com/problems/median-of-two-sorted-arrays
//
// Complexity: runtime O(logk*(logm+logn)), space O(1) [where k = range of
// possible numbers]

class Solution {
public:
    double findMedianSortedArrays(const vector<int> &nums1,
                                  const vector<int> &nums2) const
    {
        const int mi = midpoint(min<int>(nums1.size(), nums2.size()) - 1,
                                max<int>(nums1.size(), nums2.size()));

        for (int lx = -1e6, rx = 1e6;;) {
            const int mx = midpoint(lx, rx);

            const auto [lit1, rit1] =
                equal_range(nums1.begin(), nums1.end(), mx);
            const auto [lit2, rit2] =
                equal_range(nums2.begin(), nums2.end(), mx);

            const int li = (lit1 - nums1.begin()) + (lit2 - nums2.begin());
            const int ri = (rit1 - nums1.begin()) + (rit2 - nums2.begin());

            const auto num = [](const auto it, const auto &nums) -> int {
                return it != nums.end() ? *it : numeric_limits<int>::max();
            };

            if (li <= mi && ri > mi) {
                const bool odd_size = (nums1.size() + nums2.size()) % 2 == 1;
                if (odd_size || mi + 1 < ri)
                    return mx;

                return 0.5 * (mx + min(num(rit1, nums1), num(rit2, nums2)));
            }

            if (li < mi)
                lx = mx + 1;
            else if (li > mi)
                rx = mx - 1;
            else
                lx = rx = min(num(lit1, nums1), num(lit2, nums2));
        }
    }
};
