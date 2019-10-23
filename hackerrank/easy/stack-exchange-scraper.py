#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/stack-exchange-scraper

import sys
import re

ID_PAT = re.compile(r'<div class="question-summary" id="question-summary-(\d+)">')
Q_PAT = re.compile(r'<a href=".*" class="question-hyperlink">(.*)</a>')
SUB_PAT = re.compile(r'<span title=".*" class="relativetime">(.*)</span>')

if __name__ == "__main__":
    s = sys.stdin.read()
    ids, qs, subs = ID_PAT.findall(s), Q_PAT.findall(s), SUB_PAT.findall(s)
    for i in range(0, len(ids)): print(f"{ids[i]};{qs[i]};{subs[i]}")
