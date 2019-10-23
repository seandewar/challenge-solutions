#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/insertionsort1

def this_exercise_sort(arr):
    j = len(arr) - 1
    k = arr[j]
    while j > 0 and arr[j - 1] > k:
        arr[j] = arr[j - 1]
        j -= 1
        print(' '.join(map(str, arr)))
    arr[j] = k
    print(' '.join(map(str, arr)))

size = int(input())
arr = [int(arr_item) for arr_item in input().split(' ')]
this_exercise_sort(arr)
