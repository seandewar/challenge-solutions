#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/diagonal-difference

import math
import os
import random
import re
import sys

def diagonalDifference(arr):
    tot_lr, tot_rl = 0, 0
    for i in range(0, n):
        tot_lr += arr[i][i]
        tot_rl += arr[i][n - 1 - i]
    return abs(tot_lr - tot_rl)

if __name__ == '__main__':
    fptr = open(os.environ['OUTPUT_PATH'], 'w')
    n = int(input())
    arr = []
    for _ in range(n):
        arr.append(list(map(int, input().rstrip().split())))
    result = diagonalDifference(arr)
    fptr.write(str(result) + '\n')
    fptr.close()
