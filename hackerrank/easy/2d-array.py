#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/2d-array

import math
import os
import random
import re
import sys

# Complete the hourglassSum function below.
def hourglassSum(arr):
    largestsum = -sys.maxsize - 1
    for y in range(len(arr) - 2):
        for x in range(len(arr[y]) - 2):
            hgsum  = arr[y  ][x  ] + arr[y  ][x+1] + arr[y  ][x+2]
            hgsum += arr[y+1][x+1]
            hgsum += arr[y+2][x  ] + arr[y+2][x+1] + arr[y+2][x+2]
            largestsum = max(largestsum, hgsum)
    return largestsum

if __name__ == '__main__':
    fptr = open(os.environ['OUTPUT_PATH'], 'w')
    arr = []
    for _ in range(6):
        arr.append(list(map(int, input().rstrip().split())))
    result = hourglassSum(arr)
    fptr.write(str(result) + '\n')
    fptr.close()
