// https://leetcode.com/problems/maximum-swap
// Complexity: runtime O(logx), space O(1) [technically O(log(sizeof(int))), or
//                                          O(logx) if wanted].

int maximumSwap(const int num)
{
    int digits[9], ndigits = 0;
    struct Entry {
        int d, i;
    } st[9], *top = st;
    for (int x = num; x != 0; x /= 10) {
        const int d = x % 10;
        if (top == st || d > (top - 1)->d)
            *(top++) = (const struct Entry){d, ndigits};
        digits[ndigits++] = d;
    }

    int ret = 0;
    bool swapped = false;
    for (int i = ndigits - 1; i >= 0; --i) {
        if ((top - 1)->i == i)
            --top;
        if (!swapped && top != st && digits[i] < (top - 1)->d) {
            const int tmp = digits[i];
            digits[i] = digits[(top - 1)->i];
            digits[(top - 1)->i] = tmp;
            swapped = true;
        }
        ret = (ret * 10) + digits[i]; // using sprintf considered cringe
    }
    return ret;
}
