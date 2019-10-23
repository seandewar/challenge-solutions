#!/usr/bin/env python2
# https://www.hackerrank.com/challenges/ctci-contracts

# cheese with hash tables
co, n = {}, int(raw_input().strip())
for _ in xrange(n):
    op, v = raw_input().strip().split(' ')
    if op == 'add':
        for i in xrange(1, len(v) + 1): co[v[:i]] = co.get(v[:i], 0) + 1
    elif op == 'find': print co.get(v, 0)
