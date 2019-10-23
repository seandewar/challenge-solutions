#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/count-triplets-1

from collections import defaultdict
import os

def count_triplets(arr, r):
    triplets, x2d, x3d = 0, defaultdict(int), defaultdict(int)
    for x in reversed(arr):
        if x * r in x2d: triplets += x2d[x * r]
        if x * r in x3d: x2d[x] += x3d[x * r]
        x3d[x] += 1
    return triplets

if __name__ == '__main__':
    f = open(os.environ['OUTPUT_PATH'], 'w')
    nr = input().rstrip().split()
    n, r = int(nr[0]), int(nr[1])
    arr = list(map(int, input().rstrip().split()))
    f.write(str(count_triplets(arr, r)) + '\n')
