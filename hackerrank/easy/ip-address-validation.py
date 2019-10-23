#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/ip-address-validation

import re

if __name__ == '__main__':
    for _ in range(0, int(input())):
        s = input().strip()
        if re.match(r'^(([01]?\d{1,2}|2[0-4]\d|25[0-5])\.){3}([01]?\d{1,2}|2[0-4]\d|25[0-5])$', s): print('IPv4')
        elif re.match(r'^([a-f\d]{1,4}:){7}[a-f\d]{1,4}$', s): print('IPv6')
        else: print('Neither')
