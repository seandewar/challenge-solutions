// https://leetcode.com/problems/minimum-add-to-make-parentheses-valid
// Complexity: runtime O(n), space O(1).

int minAddToMakeValid(const char *s)
{
    int unclosed = 0, unopened = 0;
    for (; *s; ++s) {
        unclosed += *s == '(' ? 1 : -1;
        if (unclosed < 0) {
            unclosed = 0;
            ++unopened;
        }
    }
    return unopened + unclosed;
}
