#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/ctci-bubble-sort

import os

def bubble_sort(l):
    swaps = 0
    for i in range(0, len(l)):
        for j in range(0, len(l) - 1 - i):
            if l[j] > l[j + 1]:
                a[j], a[j + 1] = a[j + 1], a[j]
                swaps += 1
    return swaps

if __name__ == '__main__':
    n = int(input())
    a = list(map(int, input().rstrip().split()))
    print(f'Array is sorted in {bubble_sort(a)} swaps.')
    print(f'First Element: {a[0]}')
    print(f'Last Element: {a[-1]}')
