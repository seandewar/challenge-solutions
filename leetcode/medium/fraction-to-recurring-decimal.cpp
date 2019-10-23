// https://leetcode.com/problems/fraction-to-recurring-decimal/
//
// You can think of this algorithm as an implementation of the "bus stop"
// division method. This problem would be better categorised as hard instead of
// medium, in my opinion...
//
// Complexity: probably runtime O(d), space O(d)? d = denominator input

class Solution {
public:
    string fractionToDecimal(long long n, long long d)
    {
        if (n == 0)
            return "0"; // just zero
        if (d == 0)
            return "undefined"; // division by zero

        ostringstream resultss;

        if ((n < 0) ^ (d < 0))
            resultss << '-'; // write negative sign if needed

        // remove signs so they don't affect our calc - they're unneeded
        n = abs(n);
        d = abs(d);

        resultss << to_string(n / d); // write quotient

        // if there's a remainder, there's decimal places to compute...
        if (n % d != 0) {
            vector<char> digitchars;
            unordered_map<long long, vector<char>::size_type> remmap;

            // divide until there is no remainder left
            for (auto rem = n % d; rem != 0; rem %= d) {
                if (const auto fit = remmap.find(rem); fit != cend(remmap)) {
                    // already divided this remainder - digits will repeat!
                    digitchars.insert(cbegin(digitchars) + fit->second, '(');
                    digitchars.push_back(')');
                    break;
                }

                // map the remainder to the index of this digit so we know
                // where to start inserting brackets around repeating digits.
                remmap.emplace(rem, size(digitchars));

                rem *= 10;
                digitchars.push_back(to_string(rem / d).front());
            }

            resultss << '.' << string(cbegin(digitchars), cend(digitchars));
        }

        return resultss.str();
    }
};
