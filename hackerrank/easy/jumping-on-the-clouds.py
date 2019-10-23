#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/jumping-on-the-clouds

import math
import os
import random
import re
import sys

def jumpingOnClouds(clouds):
   pos, jumps = 0, 0
   while pos < len(clouds) - 1:
        c1 = clouds[pos + 1] if pos < len(clouds) - 1 else None
        c2 = clouds[pos + 2] if pos < len(clouds) - 2 else None
        pos += 2 if c2 == 0 else 1
        jumps += 1
   return jumps

if __name__ == '__main__':
    fptr = open(os.environ['OUTPUT_PATH'], 'w')
    n, c = int(input()), list(map(int, input().rstrip().split()))
    result = jumpingOnClouds(c)
    fptr.write(str(result) + '\n')
    fptr.close()
