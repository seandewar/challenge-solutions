# https://leetcode.com/problems/fibonacci-number/
#
# Uses Python 3.6. For Python 3.8 and above, you can replace reduce() by using
# assignment expressions (:= operator).
#
# Complexity: runtime O(n), space O(1).

from functools import reduce

class Solution:
    def fib(self, N: int) -> int:
        return reduce(lambda t, _ : (t[1], t[1] + t[0]), range(0, N), (0, 1))[0]
