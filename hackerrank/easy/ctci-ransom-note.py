#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/ctci-ransom-note

def ransom_note(magazine, ransom):
    magdict = {}
    for w in magazine: magdict[w] = magdict.get(w, 0) + 1
    for w in ransom:
        if magdict.get(w, 0) > 0: magdict[w] -= 1
        else: return False
    return True

m, n = map(int, input().strip().split(' '))
magazine = input().strip().split(' ')
ransom = input().strip().split(' ')
answer = ransom_note(magazine, ransom)
if(answer):
    print("Yes")
else:
    print("No")

