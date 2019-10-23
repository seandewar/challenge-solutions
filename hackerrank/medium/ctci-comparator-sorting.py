#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/ctci-comparator-sorting

from functools import cmp_to_key

class Player:
    def __init__(self, name, score):
        self.name = name
        self.score = score

    def comparator(a, b):
        if a.score < b.score: return 1
        elif a.score > b.score: return -1
        elif a.name > b.name: return 1
        elif a.name < b.name: return -1
        else: return 0
