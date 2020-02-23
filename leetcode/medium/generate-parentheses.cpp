// https://leetcode.com/problems/generate-parentheses/
//
// Uses backtracking.
// Complexity: complexity of generating n Catalan numbers: runtime
// O(4^n/sqrt(n)), space O(n+(4^n/sqrt(n))) [from the leetcode analysis]

class Solution {
    void generate(vector<string>& out, string& combination,
                  const int numRemaining, const int numUnclosed = 0) const
    {
        if (numRemaining < 0 || numUnclosed < 0)
            return; // ill-formed parens in combination

        if (numRemaining == 0 && numUnclosed == 0) {
            out.push_back(combination);
            return; // completed a valid combination
        }

        generate(out, combination += '(', numRemaining - 1, numUnclosed + 1);
        combination.back() = ')'; // backtrack the '(', replace with ')'
        generate(out, combination, numRemaining, numUnclosed - 1);
        combination.pop_back(); // backtrack the ')' for our caller
    }

public:
    vector<string> generateParenthesis(const int n) const
    {
        vector<string> results;
        string combination;

        generate(results, combination, n);
        return results;
    }
};
