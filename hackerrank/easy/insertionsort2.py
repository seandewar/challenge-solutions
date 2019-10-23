#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/insertionsort2

def insertion_sort(arr):
    if len(arr) > 1:
        for i in range(1, len(arr)):
            k = arr[i]
            j = i
            while j > 0 and arr[j - 1] > k:
                arr[j] = arr[j - 1]
                j -= 1
            arr[j] = k
            print(' '.join(map(str, arr)))

size = int(input())
arr = [int(arr_item) for arr_item in input().split(' ')]
insertion_sort(arr)
