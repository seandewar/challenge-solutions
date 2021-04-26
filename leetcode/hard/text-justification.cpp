// https://leetcode.com/problems/text-justification
//
// Complexity: runtime O(words * maxWidth), space O(maxWidth) [ignoring output].

class Solution {
public:
    vector<string> fullJustify(vector<string>& words, const int maxWidth) const
    {
        vector<string> result;
        string nextLine;
        nextLine.reserve(maxWidth);

        for (int i = 0; i < size(words);) {
            int j = i, rem = maxWidth;
            for (; j < size(words); ++j) {
                const int req = size(words[j]) + (rem < maxWidth ? 1 : 0);
                if (req > rem)
                    break;
                rem -= req;
            }

            const int gaps = j - i - 1;
            int pad = 1, extraPad = 0; // initially assume left justify
            if (j < size(words)) {
                // this isn't the last line, so don't actually left justify
                pad = 1 + (gaps > 0 ? rem / gaps : 0);
                extraPad = gaps > 0 ? rem % gaps : 0;
            }
            for (int k = i; k < j; ++k) {
                if (k > i)
                    nextLine.append(pad + (extraPad-- > 0 ? 1 : 0), ' ');
                nextLine.append(words[k]);
            }

            nextLine.resize(maxWidth, ' '); // right pad with spaces if required
            result.push_back(nextLine);
            nextLine.clear();
            i = j;
        }

        return result;
    }
};
