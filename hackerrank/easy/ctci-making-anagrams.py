#!/usr/bin/env python2
# https://www.hackerrank.com/challenges/ctci-making-anagrams

def number_needed(a, b):
    d = {}
    for c in a:
        t = d.get(c)
        if t is None: d[c] = 1
        else: d[c] += 1
    for c in b:
        t = d.get(c)
        if t is None: d[c] = -1
        else: d[c] -= 1
    return sum(abs(t) for t in d.itervalues())

a = raw_input().strip()
b = raw_input().strip()
print number_needed(a, b)
