#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/find-substring

import re

if __name__ == '__main__':
    sentences = [input().strip() for _ in range(0, int(input()))]
    queries = [input() for _ in range(0, int(input()))]
    for q in queries:
        query_matches = 0
        for s in sentences:
            query_matches += len(re.findall(r'\w' + q + r'\w', s))
        print(query_matches)
