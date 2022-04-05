/* https://leetcode.com/problems/first-unique-character-in-a-string */
/*  */
/* Complexity: runtime O(n), space O(1). */

int firstUniqChar(const char *s)
{
    int seen[26] = {0};
    for (int i = 0; *s != '\0'; ++i, ++s) {
        seen[*s - 'a'] = seen[*s - 'a'] == 0 ? i + 1 : -1;
    }
    int ret = INT_MAX;
    for (int i = 0; i < 26; ++i) {
        if (seen[i] > 0 && seen[i] - 1 < ret) {
            ret = seen[i] - 1;
        }
    }
    return ret != INT_MAX ? ret : -1;
}
