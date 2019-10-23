// https://leetcode.com/problems/defanging-an-ip-address/
//
// Uses regex. Runtime complexity for this regex should ideally be O(n).

class Solution {
public:
    string defangIPaddr(string address)
    {
        return regex_replace(address, regex("\\."), "[.]");
    }
};
