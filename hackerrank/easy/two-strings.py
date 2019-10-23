#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/two-strings

import math
import os
import random
import re
import sys
from collections import Counter

# Complete the twoStrings function below.
def twoStrings(s1, s2):
    return 'YES' if Counter(s1) & Counter(s2) else 'NO'

if __name__ == '__main__':
    fptr, q = open(os.environ['OUTPUT_PATH'], 'w'), int(input())
    for q_itr in range(q):
        s1, s2 = input(), input()
        result = twoStrings(s1, s2)
        fptr.write(result + '\n')
    fptr.close()
