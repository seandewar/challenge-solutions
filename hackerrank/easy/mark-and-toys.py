#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/mark-and-toys

import os

def max_toys(prices, k):
    result = []
    for x in sorted(prices):
        k -= x
        if k <= 0: break
        result.append(x)
    return len(result)

if __name__ == '__main__':
    fptr = open(os.environ['OUTPUT_PATH'], 'w')
    nk = input().split()
    n, k = int(nk[0]), int(nk[1])
    prices = list(map(int, input().rstrip().split()))
    result = max_toys(prices, k)
    fptr.write(str(result) + '\n')
