// https://leetcode.com/problems/check-if-a-parentheses-string-can-be-valid
//
// This one was surprisingly hard for a medium, mainly due to the second loop
// that matches unmatched '('s with unmatched unlocked parenthesis being quite
// novel to me!
//
// Complexity: runtime O(n), space O(n).

class Solution {
public:
    bool canBeValid(const string &s, const string &locked) const
    {
        if (s.size() % 2 != 0)
            return false;

        vector<int> open_stack, any_stack;
        for (int i = 0; i < s.size(); ++i) {
            if (locked[i] == '0') {
                any_stack.push_back(i);
                continue;
            }
            if (s[i] == ')') {
                // prioritize popping from the stack of locked open parenthesis,
                // as these are less flexible.
                if (!open_stack.empty()) {
                    open_stack.pop_back();
                    continue;
                }
                if (!any_stack.empty()) {
                    any_stack.pop_back();
                    continue;
                }
                return false;
            }

            open_stack.push_back(i); // s[i] == '(' && locked[i] == '1'
        }

        for (; !open_stack.empty() && !any_stack.empty(); any_stack.pop_back())
            if (open_stack.back() < any_stack.back())
                open_stack.pop_back();

        return open_stack.empty();
    }
};
