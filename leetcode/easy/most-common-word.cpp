// https://leetcode.com/problems/most-common-word
//
// Fun regex solution.
//
// This is somewhat overkill, but it's a nice excuse to whip out some regex :)
// You can simply solve this problem with some string transformations instead.
//
// Complexity: at least runtime O(n+b*m), space O(b*m) [for the matchBanned
// regex]. [where n == size(paragraph), b == size(banned), m == average size of
// words in banned]

class Solution {
public:
    string mostCommonWord(string paragraph, vector<string>& banned) const
    {
        // transform input paragraph to lowercase
        transform(begin(paragraph), end(paragraph), begin(paragraph),
                  [](const unsigned char c) { return tolower(c); });

        if (!empty(banned)) {
            // transform banned word list to lowercase
            for (auto& s : banned) {
                transform(begin(s), end(s), begin(s),
                          [](const unsigned char c) { return tolower(c); });
            }

            // build the regex we'll use to match banned words for removal
            ostringstream oss("\\b", ios_base::ate);
            for (size_t i = 0; i < size(banned); ++i) {
                if (i > 0)
                    oss << '|';

                oss << banned[i];
            }
            oss << "\\b";

            // remove banned words by replacing them with spaces, so that we
            // ignore them later
            const regex matchBanned(oss.str());
            for (sregex_iterator it(cbegin(paragraph), cend(paragraph),
                                    matchBanned);
                 it != sregex_iterator(); ++it) {
                const auto beginIt = begin(paragraph) + it->position();
                fill(beginIt, beginIt + it->length(), ' ');
            }
        }

        // count the most common words
        unordered_map<string_view, size_t> wordCounts;

        const regex matchWord("\\w+");
        for (sregex_iterator it(cbegin(paragraph), cend(paragraph), matchWord);
             it != sregex_iterator(); ++it) {
            const string_view word(paragraph.c_str() + it->position(),
                                   it->length());word
            ++wordCounts[word];
        }

        // return the most common word (guaranteed to be at least one)
        return string(max_element(cbegin(wordCounts), cend(wordCounts),
                                  [](const auto& a, const auto& b) {
                                      return a.second < b.second;
                                  })
                          ->first);
    }
};
