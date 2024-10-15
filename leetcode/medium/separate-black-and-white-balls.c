// https://leetcode.com/problems/separate-black-and-white-balls
// Complexity: runtime O(n), space O(1).

long long minimumSteps(const char *s)
{
    long long ret = 0;
    for (const char *w = s; *s; ++s)
        if (*s == '0')
            ret += s - w++;
    return ret;
}
