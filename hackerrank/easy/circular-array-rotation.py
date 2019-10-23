#!/usr/bin/env python2
# https://www.hackerrank.com/challenges/circular-array-rotation

n,k,q = raw_input().strip().split(' ')
n,k,q = [int(n),int(k),int(q)]
a = map(int,raw_input().strip().split(' '))
for a0 in xrange(q):
    m = int(raw_input().strip())
    print a[(m - k) % len(a)]
