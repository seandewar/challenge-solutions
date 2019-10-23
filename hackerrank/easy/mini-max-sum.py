#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/mini-max-sum

import math
import os
import random
import re
import sys

# Complete the miniMaxSum function below.
def miniMaxSum(arr):
    arr.sort()
    print(sum(arr[:4]), sum(arr[-4:]))

if __name__ == '__main__':
    arr = list(map(int, input().rstrip().split()))
    miniMaxSum(arr)
