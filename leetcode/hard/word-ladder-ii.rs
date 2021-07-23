// https://leetcode.com/problems/word-ladder-ii
//
// Very hard problem; solution has room for improvement, but runs in ~0ms so *shrug*.
//
// The crux of this problem is that we can't use a standard BFS to traverse the full graph,
// otherwise we will have way too many paths to consider and will eventually TLE or MLE.
//
// The idea instead is to truncate the graph, so that the max amount of edges (max depth of our
// graph) we would ever need to traverse from the begin_word is the min amount of moves needed to
// transform begin_word to end_word. We also need to ensure our graph is directional only towards
// lower depths and that no cycles are possible. By doing this, we can find the valid paths by
// simply DFSing (yes, depth-first searching) with backtracking, without the need for tracking
// previous visits during that process.
//
// Complexity: too busy. :^)

use std::{
    collections::{HashMap, HashSet, VecDeque},
    iter::once,
    mem::replace,
};

impl Solution {
    pub fn find_ladders(
        begin_word: String,
        end_word: String,
        word_list: Vec<String>,
    ) -> Vec<Vec<String>> {
        let similiars = once(begin_word.as_str())
            .chain(word_list.iter().map(|w| w.as_str()))
            .fold(HashMap::new(), |mut acc, w| {
                for i in 0..w.len() {
                    let mut k: Vec<_> = w.as_bytes().to_vec();
                    k[i] = b'*';
                    acc.entry(k).or_insert(vec![]).push(w);
                }
                acc
            });

        let mut graph = HashMap::<_, HashSet<_>>::new();
        {
            let mut visited = HashSet::new();
            let mut queue: VecDeque<_> = once((begin_word.as_str(), None)).collect();
            while !visited.contains(end_word.as_str()) && !queue.is_empty() {
                let mut newly_visited = HashSet::new();
                for _ in 0..queue.len() {
                    let (word, from_word) = queue.pop_front().unwrap();
                    if visited.contains(word) {
                        continue;
                    }
                    if let Some(w) = from_word {
                        if graph.get(word).map_or(true, |words| !words.contains(w)) {
                            graph.entry(w).or_insert(HashSet::new()).insert(word);
                        }
                    }
                    if !newly_visited.insert(word) || word == end_word {
                        continue;
                    }
                    let mut k: Vec<_> = word.as_bytes().to_vec();
                    for i in 0..word.len() {
                        let old_char = replace(&mut k[i], b'*');
                        similiars
                            .get(&k)
                            .iter()
                            .flat_map(|words| words.iter())
                            .filter(|&&w| word != w)
                            .for_each(|&w| queue.push_back((w, Some(word))));
                        k[i] = old_char;
                    }
                }
                visited.extend(newly_visited.drain());
            }
        }

        let mut results: Vec<Vec<_>> = vec![];
        let mut path = vec![begin_word.as_str()];
        Self::solve(&graph, &end_word, &mut path, &mut results);
        results
    }

    fn solve<'a>(
        graph: &HashMap<&'a str, HashSet<&'a str>>,
        end_word: &'a str,
        path: &mut Vec<&'a str>,
        out: &mut Vec<Vec<String>>,
    ) {
        let &word = path.last().unwrap();
        if word == end_word {
            out.push(path.iter().map(|w| w.to_string()).collect());
            return;
        }
        graph
            .get(word)
            .iter()
            .flat_map(|words| words.iter())
            .for_each(|&w| {
                path.push(w);
                Self::solve(graph, end_word, path, out);
                path.pop();
            });
    }
}
