// https://leetcode.com/problems/regular-expression-matching
//
// Iterative solution, with recursion for non-greedy '*' matches.
// Yes, you can cheese this with std::regex, but where's the fun in that?!
// Runtime can be improved with dynamic programming.
//
// Complexity: kinda complicated.

class Solution {
public:
    bool isMatch(string_view s, string_view p) const noexcept
    {
        while (!empty(p)) {
            const bool repeat = size(p) > 1 && p[1] == '*';

            // match failure
            if (empty(s) || (p[0] != '.' && p[0] != s[0])) {
                if (!repeat)
                    return false;

                p.remove_prefix(2); // skip the * too
                continue;
            }

            // match success
            if (!repeat)
                p.remove_prefix(1);
            else {
                // * is non-greedy in this problem, as opposed to standard regex
                // which is greedy by default; therefore, we also need to try
                // ending the * match early to see if we can match s that way
                if (isMatch(s, p.substr(2)))
                    return true;
            }

            s.remove_prefix(1);
        }

        return empty(s);
    }
};
