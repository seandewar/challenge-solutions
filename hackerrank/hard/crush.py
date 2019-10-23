#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/crush

import math
import os
import random
import re
import sys

# Complete the arrayManipulation function below.
def arrayManipulation(n, queries):
    operations = [0] * n
    for q in queries:
        a, b, k = q[0] - 1, q[1], q[2]
        operations[a] += k
        if b < n: operations[b] -= k

    op, maxval = 0, 0
    for i in range(0, n):
        op += operations[i]
        maxval = max(maxval, op)

    return maxval

if __name__ == '__main__':
    fptr = open(os.environ['OUTPUT_PATH'], 'w')
    nm = input().split()
    n = int(nm[0])
    m = int(nm[1])
    queries = []
    for _ in range(m):
        queries.append(list(map(int, input().rstrip().split())))
    result = arrayManipulation(n, queries)
    fptr.write(str(result) + '\n')
    fptr.close()
