#!/usr/bin/env python2
# https://www.hackerrank.com/challenges/ctci-array-left-rotation

def array_left_rotation(a, d):
    return a[d:] + a[:d]

_, d = map(int, raw_input().strip().split(' '))
a = map(int, raw_input().strip().split(' '))
print ' '.join(map(str, array_left_rotation(a, d)))
