// https://leetcode.com/problems/implement-rand10-using-rand7
//
// Uses rejection sampling to generate a uniform value.
//
// We use 2 calls to rand7() to uniformly generate a number in the range [1,
// 7*7=49], rejecting and re-rolling values in [41, 49] so that we only accept
// uniform values in [1, 40], so that we can modulo these values to obtain a
// uniform value in [1, 10].
//
// The leetcode solution article explains this in more detail if required.
//
// Complexity: runtime O(infinity) [O(1) average], space O(1).

// The rand7() API is already defined for you.
// int rand7();
// @return a random integer in the range 1 to 7

int rand10(void)
{
    int x;
    while ((x = rand7() + (rand7() - 1) * 7) > 40);
    return 1 + (x - 1) % 10;
}
