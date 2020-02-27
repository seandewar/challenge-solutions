// https://leetcode.com/problems/group-anagrams/
//
// Groups anagrams together via a hash map of anagrams paired with their sorted
// versions. Anagrams that should be together will produce the same result after
// the sort, and thus, will be grouped together under the same key.
//
// Complexity: runtime O(n*mlogm), space O(n*m) [where n=size(strs), m=longest
// individual string size in strs].

class Solution {
public:
    vector<vector<string>> groupAnagrams(const vector<string>& strs) const
    {
        unordered_map<string, vector<string>> anagramMap;

        for (const auto& s : strs) {
            auto sorted = s;
            sort(begin(sorted), end(sorted));
            anagramMap[sorted].push_back(s);
        }

        vector<vector<string>> results;

        for (auto& kv : anagramMap)
            results.push_back(move(kv.second));

        return results;
    }
};

// Alternative Solution: counting sort over std::sort()
//
// Because the test input may only contain lower-case letters (26 different
// chars), counting sort may be used instead to reduce the runtime complexity.
//
// Complexity: runtime O(n*m), space O(n*m).
/*
class Solution {
public:
    vector<vector<string>> groupAnagrams(const vector<string>& strs) const
    {
        unordered_map<string, vector<string>> anagramMap;

        {
            array<string::size_type, 26> charCounts;

            for (const auto& s : strs) {
                fill(begin(charCounts), end(charCounts), 0);

                for (const char c : s)
                    ++charCounts[c - 'a'];

                string sorted;
                sorted.reserve(size(s));

                for (char i = 0; i < size(charCounts); ++i)
                    sorted += string(charCounts[i], 'a' + i);

                anagramMap[sorted].push_back(s);
            }
        }

        vector<vector<string>> results;

        for (auto& kv : anagramMap)
            results.push_back(move(kv.second));

        return results;
    }
};
*/
