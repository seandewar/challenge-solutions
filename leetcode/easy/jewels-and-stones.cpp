// https://leetcode.com/problems/jewels-and-stones/
//
// Using a hash set & std::count_if().
// Complexity: runtime O(n+m), space O(n) [where n = size(J), m = size(S)].

class Solution {
public:
    int numJewelsInStones(const string& J, const string& S) const {
        const unordered_set jewels(cbegin(J), cend(J));
        return count_if(cbegin(S), cend(S), [&](const char s) {
            return jewels.count(s) > 0;
        });
    }
};
