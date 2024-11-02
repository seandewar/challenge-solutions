// https://leetcode.com/problems/circular-sentence
// Complexity: runtime O(n), space O(1).

class Solution {
public:
    bool isCircularSentence(const string &sentence) const
    {
        if (sentence.front() != sentence.back())
            return false;
        for (auto i = sentence.find_first_of(' '); i < sentence.size();
             i = sentence.find_first_of(' ', i + 1)) {
            if (sentence[i - 1] != sentence[i + 1])
                return false;
        }
        return true;
    }
};
