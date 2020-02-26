// https://leetcode.com/problems/evaluate-reverse-polish-notation/
//
// This is the first solution that came to mind. See my alternative solution for
// an optimized version (same runtime/space complexity).
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

// Alternative Solution: operation to function hash map + std::from_chars().
// Same complexity, but slightly faster than the above.
//
// Complexity: runtime O(n), space O(n).
/*
class Solution {
public:
    int evalRPN(const vector<string>& tokens) const
    {
        static const unordered_map<string_view, function<int (int, int)>>
            opMap = {
                {"+", plus<int>()},
                {"-", minus<int>()},
                {"*", multiplies<int>()},
                {"/", divides<int>()}
            };

        stack<int> s;

        for (const string_view t : tokens) { // implicit conv to string_view
            if (const auto fIt = opMap.find(t); fIt != cend(opMap)) { // op
                const int b = s.top();
                s.pop();

                const int a = s.top();
                s.pop();

                s.push(fIt->second(a, b));
            } else { // assume valid int
                int val;
                from_chars(t.data(), t.data() + size(t), val);
                s.push(val);
            }
        }

        return !empty(s) ? s.top() : 0;
    }
};
*/
