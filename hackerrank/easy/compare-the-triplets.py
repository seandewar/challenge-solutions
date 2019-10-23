#!/usr/bin/env python2
# https://www.hackerrank.com/challenges/compare-the-triplets

ar = [int(e) for e in raw_input().strip().split(' ')]
br = [int(e) for e in raw_input().strip().split(' ')]
a, b = 0, 0
for i in xrange(0, 3):
    if ar[i] > br[i]: a += 1
    elif ar[i] < br[i]: b += 1
print a, b
