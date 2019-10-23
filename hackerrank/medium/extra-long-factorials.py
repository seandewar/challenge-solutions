#!/usr/bin/env python2
# https://www.hackerrank.com/challenges/extra-long-factorials

# python cheeses this problem
N = int(raw_input().strip())
answer = 1
for i in xrange(1, N + 1): answer *= i
print answer
