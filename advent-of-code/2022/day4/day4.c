// Bonus C solution! I originally wrote this on my phone (using Termux!)
// Reads from stdin.
#include <stdio.h>
int main(void)
{
    int p1 = 0, p2 = 0;
    for (int a, b, x, y; scanf("%d-%d,%d-%d", &a, &b, &x, &y) != EOF;) {
        p1 += (a <= x && b >= y) || (x <= a && y >= b);
        p2 += a <= y && x <= b;
    }
    printf("Day4 (C99): P1: %d, P2: %d\n", p1, p2);
}
