// https://leetcode.com/problems/search-suggestions-system
//
// (Associated) Trie-Based Solution.
// Slightly over-engineered, but I wanted to really use a trie for this one. :)
//
// Complexity: runtime O(p*l*logp), space O(s) [where s == search_word.len(), p == products.len(),
//                                              l == average product string length]

#[derive(Default)]
struct TrieNode {
    children: [Option<Box<TrieNode>>; 26],
    product_indices: Vec<usize>,
}

impl Solution {
    pub fn suggested_products(mut products: Vec<String>, search_word: String) -> Vec<Vec<String>> {
        let idx = |c| c as usize - 'a' as usize;
        products.sort_unstable();
        let mut trie = TrieNode::default();
        for (i, p) in products.iter().enumerate() {
            let mut node = &mut trie;
            for c in p.chars() {
                node = node.children[idx(c)].get_or_insert_with(|| Box::default());
                node.product_indices.push(i);
            }
        }

        search_word
            .chars()
            .scan(Some(&trie), |state, c| {
                if let Some(node) = *state {
                    if let Some(ref node) = node.children[idx(c)] {
                        *state = Some(node);
                        return Some(
                            node.product_indices
                                .iter()
                                .take(3)
                                .map(|&i| products[i].clone())
                                .collect(),
                        );
                    }
                }
                *state = None;
                Some(vec![])
            })
            .collect()
    }
}
