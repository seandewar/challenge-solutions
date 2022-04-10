/* https://leetcode.com/problems/baseball-game */
/*  */
/* Complexity: runtime O(n), space O(1). */

int calPoints(const char *const *const ops, const int n)
{
    int scores[1001], scoresn = 0;
    for (int i = 0; i < n; ++i) {
        switch (ops[i][0]) {
        case '+':
            scores[scoresn] = scores[scoresn - 1] + scores[scoresn - 2];
            ++scoresn;
            break;
        case 'D':
            scores[scoresn] = 2 * scores[scoresn - 1];
            ++scoresn;
            break;
        case 'C':
            --scoresn;
            break;
        default:
            scores[scoresn++] = atoi(ops[i]);
            break;
        }
    }

    int ret = 0;
    for (int i = 0; i < scoresn; ++i)
        ret += scores[i];

    return ret;
}
