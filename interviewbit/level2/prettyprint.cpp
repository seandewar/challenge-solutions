// https://www.interviewbit.com/problems/prettyprint/
//
// Complexity: runtime O(a^2), space O(1) [output not considered].

vector<vector<int>> Solution::prettyPrint(const int A)
{
    if (A <= 0)
        return {};

    const int N = A * 2 - 1;
    vector<vector<int>> result(N, vector<int>(N));

    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < N; ++x)
            result[y][x] = max(abs(y - N / 2), abs(x - N / 2)) + 1;
    }

    return result;
}
