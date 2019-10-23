#!/usr/bin/env python3
# https://www.hackerrank.com/challenges/grading

import os
import sys

# Complete the gradingStudents function below.
def gradingStudents(grades):
    result = []
    for g in grades:
        if g >= 38:
            r = 5 - (g % 5)
            if r <= 2: g += r
        result.append(g)
    return result

if __name__ == '__main__':
    f = open(os.environ['OUTPUT_PATH'], 'w')
    n = int(input())
    grades = []
    for _ in range(n):
        grades_item = int(input())
        grades.append(grades_item)
    result = gradingStudents(grades)
    f.write('\n'.join(map(str, result)))
    f.write('\n')
    f.close()
