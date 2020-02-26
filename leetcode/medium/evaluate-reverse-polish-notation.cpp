// https://leetcode.com/problems/evaluate-reverse-polish-notation/
//
// Complexity: runtime O(n), space O(n).

class Solution {
public:
    int evalRPN(const vector<string>& tokens) const
    {
        stack<int> s;

        for (const auto& t : tokens) {
            if (int val; istringstream(t) >> val) {
                s.push(val); // valid integer
            } else {
                const int b = s.top();
                s.pop();

                const int a = s.top();
                s.pop();

                if (t == "+")
                    s.push(a + b);
                else if (t == "-")
                    s.push(a - b);
                else if (t == "*")
                    s.push(a * b);
                else if (t == "/")
                    s.push(a / b);
            }
        }

        return !empty(s) ? s.top() : 0;
    }
};
