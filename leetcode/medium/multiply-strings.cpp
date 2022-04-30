// https://leetcode.com/problems/multiply-strings
// Complexity: runtime O(n*(n+m)), space O(n+m).

class Solution {
public:
    string multiply(const string num1, const string num2) const
    {
        string acc("0");
        for (string::size_type i = 0; i < size(num2); ++i) {
            string val("0");
            for (string::size_type j = 0; j < size(num1); ++j) {
                const unsigned res = (val[j] - '0')
                                     + (num2[size(num2) - i - 1] - '0')
                                           * (num1[size(num1) - j - 1] - '0');
                val.back() = res % 10 + '0';
                val.push_back(res / 10 + '0');
            }

            reverse(begin(val), end(val));
            val.append(i, '0');
            string new_acc;
            for (bool carry = false; !empty(val) || !empty(acc) || carry;) {
                unsigned a = 0, b = 0;
                if (!empty(val)) {
                    a = val.back() - '0';
                    val.pop_back();
                }
                if (!empty(acc)) {
                    b = acc.back() - '0';
                    acc.pop_back();
                }

                const unsigned res = a + b + carry;
                new_acc.push_back(res % 10 + '0');
                carry = res / 10;
            }

            new_acc.erase(
                find_if_not(rbegin(new_acc), rend(new_acc),
                            [](const auto v) noexcept { return v == '0'; })
                    .base(),
                end(new_acc));
            reverse(begin(new_acc), end(new_acc));
            acc = empty(new_acc) ? "0" : move(new_acc);
        }

        return acc;
    }
};
