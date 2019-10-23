// https://www.hackerrank.com/challenges/find-substring

#include <iostream>
#include <regex>
#include <utility>
#include <vector>

using namespace std;

int main() {
    size_t numSentences;
    cin >> numSentences >> ws;

    vector<string> sentences;
    sentences.reserve(numSentences);

    for (size_t i = 0; i < numSentences; ++i) {
        string sentence;
        getline(cin, sentence);
        sentences.push_back(move(sentence));
    }

    size_t numQueries;
    cin >> numQueries >> ws;

    for (size_t i = 0; i < numQueries; ++i) {
        string query;
        getline(cin, query);

        size_t match_count = 0;
        for (const auto &s : sentences) {
            regex regex("\\w" + query + "\\w");

            for (auto it = sregex_iterator(cbegin(s), cend(s), regex);
                 it != sregex_iterator();
                 ++it)
                ++match_count;
        }

        cout << match_count << "\n";
    }
}
