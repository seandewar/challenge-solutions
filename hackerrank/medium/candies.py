#!/usr/bin/env python2
# https://www.hackerrank.com/challenges/candies

N = int(raw_input().strip())
ratings = [int(raw_input().strip()) for _ in xrange(0, N)]
candies = [1 for _ in xrange(0, len(ratings))]
for i in xrange(1, len(ratings)):
    if ratings[i] > ratings[i - 1] and candies[i] <= candies[i - 1]:
        candies[i] = candies[i - 1] + 1
for i in xrange(len(ratings) - 2, -1, -1):
    if ratings[i] > ratings[i + 1] and candies[i] <= candies[i + 1]:
        candies[i] = candies[i + 1] + 1
print sum(candies)
