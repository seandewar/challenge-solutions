# https://leetcode.com/problems/largest-component-size-by-common-factor/
#
# Uses a dictionary to represent a disjoint set, utilizing union and find
# operations to pass the time complexity requirement.
#
# Initially, I re-created the graph using a dictionary of nodes and their
# connections via gcd(), then performed a DFS to find the largest component
# size, but this resulted in TLE, as the time complexity was O(n^2).
#
# This still has a lot of room for optimization.
#
# Complexity: Idk. I wrote this a while ago and I forgot to fill this in...
#             I'll fill this out the next time I do this problem. :P

from typing import Dict

class Solution:
    def largestComponentSize(self, A: List[int]) -> int:
        ds = {}

        def ds_find(ds: Dict[int, int], x: int) -> int:
            if ds.setdefault(x, x) != x:
                ds[x] = ds_find(ds, ds[x])

            return ds[x]

        for x in A:
            for i in range(2, int(sqrt(x)) + 1):
                if x % i == 0:
                    ds[ds_find(ds, i)] = ds_find(ds, x)
                    ds[ds_find(ds, i)] = ds_find(ds, x // i)

        return max(Counter(ds_find(ds, i) for i in A).values())
