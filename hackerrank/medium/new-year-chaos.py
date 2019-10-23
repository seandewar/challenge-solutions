#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/new-year-chaos

def minimum_bribes(q):
    minbribes, i = 0, len(q) - 1
    while i >= 0:
        diff = q[i] - i - 1
        if diff > 2: return None
        elif diff == 2: q[i], q[i+1], q[i+2] = q[i+1], q[i+2], q[i]
        elif diff == 1: q[i], q[i+1] = q[i+1], q[i]
        i += max(diff, 0) - 1
        minbribes += max(diff, 0)
    return minbribes

if __name__ == '__main__':
    for t_itr in range(int(input())):
        n, q = int(input()), list(map(int, input().rstrip().split()))
        bribes = minimum_bribes(q)
        print('Too chaotic' if bribes is None else str(bribes))
