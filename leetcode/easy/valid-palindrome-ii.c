// https://leetcode.com/problems/valid-palindrome-ii
//
// Complexity: runtime O(n), space O(1).

bool validPalindrome(const char *s)
{
    const char *e = strchr(s, '\0') - 1, *saves = NULL, *savee = NULL;
    while (s < e) {
        if (*s == *e) {
            ++s;
            --e;
            continue;
        }
        if (!saves) {  // try "deleting" char at s
            saves = s++;
            savee = e;
        } else if (savee) {  // try "deleting" char at e
            s = saves;
            e = savee - 1;
            savee = NULL;
        } else {  // both failed
            return false;
        }
    }
    return true;
}
