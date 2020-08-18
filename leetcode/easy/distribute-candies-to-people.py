# https://leetcode.com/problems/distribute-candies-to-people
#
# Simulates the distribution of candies. Also possible to do this mathematically
# with O(num_people) runtime.
#
# Complexity: runtime O(sqrt(c)), space O(1) [where c = num candies].

class Solution:
    def distributeCandies(self, candies: int, num_people: int) -> List[int]:
        res = [0] * num_people
        try_give = 1

        while candies > 0:
            give = min(candies, try_give)
            res[(try_give - 1) % len(res)] += give
            candies -= give
            try_give += 1

        return res
