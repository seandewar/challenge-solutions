#!/usr/bin/env python2
# https://www.hackerrank.com/challenges/simple-array-sum

import sys

n = int(raw_input().strip())
arr = map(int,raw_input().strip().split(' '))
print sum(arr)
