// https://leetcode.com/problems/add-strings
//
// Complexity: runtime O(n+m), space O(n+m).

class Solution {
public:
    string addStrings(string num1, string num2) const
    {
        string result;
        for (bool carry = false; !empty(num1) || !empty(num2) || carry;) {
            int a = 0, b = 0;
            if (!empty(num1)) {
                a = num1.back() - '0';
                num1.pop_back();
            }
            if (!empty(num2)) {
                b = num2.back() - '0';
                num2.pop_back();
            }

            const int c = a + b + carry;
            result.push_back('0' + c % 10);
            carry = c > 9;
        }

        reverse(begin(result), end(result));
        return result;
    }
};
