#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/plus-minus

import math
import os
import random
import re
import sys

# Complete the plusMinus function below.
def plusMinus(arr):
    p, m = 0, 0
    for e in arr:
        if e > 0: p += 1
        elif e < 0: m += 1
    print(float(p / len(arr)))
    print(float(m / len(arr)))
    print(1.0 - float((p + m) / len(arr)))

if __name__ == '__main__':
    n = int(input())
    arr = list(map(int, input().rstrip().split()))
    plusMinus(arr)
