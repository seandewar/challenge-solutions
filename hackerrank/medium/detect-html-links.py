#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/detect-html-links

import re
PATT = re.compile(r'<a href="(.*?)".*?>\s*([\w.,/ ]*)\s*</.*?>')

if __name__ == '__main__':
    src = ''.join([input() for _ in range(0, int(input()))])
    for (link, text) in PATT.findall(src): print(f"{link},{text}")
