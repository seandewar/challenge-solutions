// https://leetcode.com/problems/longest-happy-string
// Complexity: runtime O(a+b+c), space O(1).

class Solution {
public:
    string longestDiverseString(const int a, const int b, const int c) const
    {
        string ret;
        array<pair<int, char>, 3> s{pair{a, 'a'}, {b, 'b'}, {c, 'c'}};

        while (true) {
            sort(s.begin(), s.end(), greater{});
            auto &[n0, c0] = s[0];
            auto &[n1, c1] = s[1];

            if (n1 == 0) {
                if (n0 > 1)
                    ret.push_back(c0);
                if (n0 > 0)
                    ret.push_back(c0);
                break;
            }
            if (n0 > n1) { // diff of 1 instead also creates a valid answer
                --n0;
                ret.push_back(c0);
            }
            --n0;
            ret.push_back(c0);
            --n1;
            ret.push_back(c1);
        }

        return ret;
    }
};
