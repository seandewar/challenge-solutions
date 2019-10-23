#!/usr/bin/env python2
# https://www.hackerrank.com/challenges/journey-to-the-moon

N, I = [int(e) for e in raw_input().strip().split()]
astros_by_country = [set([n]) for n in xrange(0, N)]
for i in xrange(0, I):
    p = set([int(e) for e in raw_input().strip().split()])
    found_in = None
    for a in astros_by_country:
        if p & a:
            if found_in is None:
                a |= p
                found_in = a
            else:
                found_in |= a
                a.clear()
num_pairs = N * (N - 1) / 2
for a in astros_by_country:
    num_pairs -= len(a) * (len(a) - 1) / 2
print num_pairs
