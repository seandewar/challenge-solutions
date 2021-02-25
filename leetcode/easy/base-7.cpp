// https://leetcode.com/problems/base-7
//
// Complexity: runtime O(logn), space O(logn).

class Solution {
public:
    string convertToBase7(int num) const
    {
        // we're creating the string in reverse order, so need to we'll reverse
        // it after. NOTE: we init the oss buf to "0" so that num == 0 defaults
        // to "0", as the std::ios_base::ate flag isn't set by default, which
        // causes writes to overwrite the initial "0", which happens if num != 0
        const bool neg = num < 0;
        ostringstream oss("0");
        for (; num != 0; num /= 7)
            oss << abs(num) % 7;
        if (neg)
            oss << '-';

        auto result = oss.str();
        reverse(begin(result), end(result));
        return result;
    }
};
