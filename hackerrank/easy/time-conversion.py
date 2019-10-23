#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/time-conversion

import os
import sys

# Complete the timeConversion function below.
def timeConversion(s):
    hs, ms, ss = s.split(':')
    h, m, sn = int(hs), int(ms), int(ss[0:2])
    if s[-2:] == 'PM':
        if h != 12: h += 12
    elif h == 12: h = 0
    return '{:02d}:{:02d}:{:02d}'.format(h,m,sn)

if __name__ == '__main__':
    f = open(os.environ['OUTPUT_PATH'], 'w')
    s = input()
    result = timeConversion(s)
    f.write(result + '\n')
    f.close()
