#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/sock-merchant

import math
import os
import random
import re
import sys

# Complete the sockMerchant function below.
def sockMerchant(n, ar):
    sockpairs = {}
    for s in ar: sockpairs[s] = sockpairs.get(s, 0) + 1
    numpairs = 0
    for (_, n) in sockpairs.items(): numpairs += n // 2
    return numpairs

if __name__ == '__main__':
    fptr = open(os.environ['OUTPUT_PATH'], 'w')
    n = int(input())
    ar = list(map(int, input().rstrip().split()))
    result = sockMerchant(n, ar)
    fptr.write(str(result) + '\n')
    fptr.close()
