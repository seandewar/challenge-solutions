#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/frequency-queries

import os

def freq_query(qs):
    d, s, a = {}, {0: 0}, []
    for q in qs:
        t, x = q[0], q[1]
        if t == 1:
            d[x] = d.get(x, 0) + 1
            s[d[x]] = s.get(d[x], 0) + 1
            s[d[x]-1] = max(0, s[d[x]-1] - 1)
        elif t == 2 and d.get(x, 0) > 0:
            d[x] -= 1
            s[d[x]] += 1
            s[d[x]+1] -= 1
        elif t == 3: a.append(1 if s.get(x, 0) > 0 else 0)
    return a

if __name__ == '__main__':
    f, n = open(os.environ['OUTPUT_PATH'], 'w'), int(input().strip())
    qs = [list(map(int, input().rstrip().split())) for _ in range(n)]
    f.write('\n'.join(map(str, freq_query(qs))) + '\n')
