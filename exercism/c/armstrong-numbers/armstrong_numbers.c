#include "armstrong_numbers.h"

bool is_armstrong_number(int candidate)
{
    int digits = 1;
    int x = candidate;
    while (x > 9 || x < -9) {
        x /= 10;
        ++digits;
    }

    int r = 0;
    x = candidate;
    while (x) {
        int d = 1;
        for (int i = 0; i < digits; ++i)
            d *= x % 10;
        r += d;
        x /= 10;
    }

    return r == candidate;
}
