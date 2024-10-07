// https://leetcode.com/problems/minimum-string-length-after-removing-substrings
// Complexity: runtime O(n), space O(n).

int minLength(const char *s)
{
    char stack[100], *top = stack;
    for (; *s; ++s) {
        if (top > stack
            && ((*s == 'B' && *(top - 1) == 'A')
                || (*s == 'D' && *(top - 1) == 'C')))
            --top;
        else
            *(top++) = *s;
    }
    return top - stack;
}
