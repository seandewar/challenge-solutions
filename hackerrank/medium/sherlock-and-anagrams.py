#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/sherlock-and-anagrams

import math
import os
import random
import re
import sys
from collections import Counter

# Complete the sherlockAndAnagrams function below.
def sherlockAndAnagrams(s):
    lettercounts, result = Counter(s), 0
    for sublen in range(1, len(s)):
        subs = {}
        for subi in range(0, len(s) - sublen + 1):
            sub = list(s[subi:subi + sublen])
            sub.sort()
            sub = ''.join(sub)
            if sub in subs: subs[sub] += 1
            else: subs[sub] = 1
            result += subs[sub] - 1
    return result

if __name__ == '__main__':
    fptr = open(os.environ['OUTPUT_PATH'], 'w')
    q = int(input())
    for q_itr in range(q):
        s = input()
        result = sherlockAndAnagrams(s)
        fptr.write(str(result) + '\n')
    fptr.close()
