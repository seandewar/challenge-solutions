// https://leetcode.com/problems/divide-players-into-teams-of-equal-skill
// Complexity: runtime O(nlogn), space O(logn).

static int cmp(const void *const a, const void *const b)
{
    return *(const int *)a - *(const int *)b;
}

long long dividePlayers(int *const skill, const int n)
{
    qsort(skill, n, sizeof skill[0], cmp);
    long long sum = 0;
    for (int i = 0; i < n / 2; ++i) {
        if (skill[i] + skill[n - 1 - i] != skill[0] + skill[n - 1])
            return -1;
        sum += (long long)skill[i] * skill[n - 1 - i];
    }
    return sum;
}
