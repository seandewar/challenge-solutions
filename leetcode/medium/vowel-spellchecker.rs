// https://leetcode.com/problems/vowel-spellchecker
//
// Complexity: runtime O(q+w), space O(q+w) [where q == queries.len(), w == wordlist.len();
// excluding character data]

use std::collections::{HashMap, HashSet};

impl Solution {
    pub fn spellchecker(wordlist: Vec<String>, queries: Vec<String>) -> Vec<String> {
        fn hide_vowels(w_low: &str) -> String {
            assert!(w_low.chars().all(char::is_lowercase));
            w_low.replace(|c| "aeiou".contains(c), " ")
        }

        let wordset: HashSet<_> = wordlist.iter().map(String::as_str).collect();
        let (mut low_map, mut novowel_map) = (HashMap::new(), HashMap::new());
        wordlist
            .iter()
            .map(|w| (w.as_str(), w.to_lowercase()))
            .for_each(|(w, w_low)| {
                novowel_map.entry(hide_vowels(&w_low)).or_insert(w);
                low_map.entry(w_low).or_insert(w);
            });

        queries
            .into_iter()
            .map(|q| {
                let q_low = q.to_lowercase();
                wordset
                    .get(q.as_str())
                    .or_else(|| low_map.get(&q_low))
                    .or_else(|| novowel_map.get(&hide_vowels(&q_low)))
                    .unwrap_or(&"")
                    .to_string()
            })
            .collect()
    }
}

// Extra: An even better spell-checker!
//
// Among much more sensible precedence rules, this spellchecker also applies the same case as the
// query in vowel corrections!

use std::collections::HashMap;

#[derive(Default)]
struct TrieNode<'a> {
    children: HashMap<char, Box<Self>>,
    word: Option<&'a str>,
}

impl Solution {
    pub fn spellchecker(wordlist: Vec<String>, queries: Vec<String>) -> Vec<String> {
        let mut trie = TrieNode::default();
        for w in &wordlist {
            w.chars()
                .fold(&mut trie, |node, c| node.children.entry(c).or_default())
                .word = Some(w);
        }

        queries
            .into_iter()
            .map(|q| Self::dfs_match(&trie, &q, None).unwrap_or_default().into())
            .collect()
    }

    fn dfs_match<'a>(node: &'a TrieNode, query: &str, replace: Option<char>) -> Option<&'a str> {
        if query.is_empty() {
            node.word.clone()
        } else {
            let c = replace.or_else(|| query.chars().next()).unwrap();
            let c_map_case = |x: char, invert| {
                if c.is_ascii_uppercase() ^ invert {
                    x.to_ascii_uppercase()
                } else {
                    x.to_ascii_lowercase()
                }
            };

            if let Some(next) = node
                .children
                .get(&c)
                .or_else(|| node.children.get(&c_map_case(c, true)))
            {
                // exact match - same or differing case
                Self::dfs_match(next, &query[1..], None)
            } else if replace.is_none() {
                // vowel match - same or differing case
                "aeiou"
                    .chars()
                    .find_map(|v| Self::dfs_match(node, query, Some(c_map_case(v, false))))
            } else {
                None
            }
        }
    }
}
