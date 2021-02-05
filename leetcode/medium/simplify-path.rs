// https://leetcode.com/problems/simplify-path
//
// Complexity: runtime O(n), space O(n).

impl Solution {
    pub fn simplify_path(path: String) -> String {
        let mut dir_stack = Vec::new();
        for s in path.split("/").filter(|s| s != &"" && s != &".") {
            match s {
                ".." => { dir_stack.pop(); }
                s => dir_stack.push(s),
            }
        }
        "/".to_owned() + &dir_stack.join("/")
    }
}
