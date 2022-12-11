// Reads from stdin
use std::{cell::RefCell, rc::Rc};

fn main() {
    #[derive(Default)]
    struct Node {
        name: String,
        size: usize,
        children: Vec<Rc<RefCell<Node>>>,
    }
    let root = Rc::new(RefCell::new(Node::default()));
    let mut path = vec![Rc::clone(&root)];
    for line in std::io::stdin().lines().skip(1).map(|l| l.unwrap()) {
        let cwd = path.last().unwrap();
        if let Some(name) = line.strip_prefix("dir ") {
            cwd.borrow_mut().children.push(Rc::new(RefCell::new(Node {
                name: name.into(),
                ..Node::default()
            })));
        } else if line.chars().next().unwrap().is_ascii_digit() {
            let size = line
                .split_once(' ')
                .map(|(s, _)| s.parse::<usize>().unwrap())
                .unwrap();
            for node in &path {
                node.borrow_mut().size += size;
            }
        } else if let Some(name) = line.strip_prefix("$ cd ") {
            if name == ".." {
                path.pop();
                continue;
            }
            let node = Rc::clone(
                cwd.borrow()
                    .children
                    .iter()
                    .find(|n| n.borrow().name == name)
                    .unwrap(),
            );
            path.push(node);
        }
    }
    fn dfs(node: &RefCell<Node>, space_req: usize) -> (usize, usize) {
        let size = node.borrow().size;
        node.borrow().children.iter().fold(
            (
                if size <= 100_000 { size } else { 0 },
                if size >= space_req { size } else { usize::MAX },
            ),
            |(p1, p2), child| {
                let (child_p1, child_p2) = dfs(child.as_ref(), space_req);
                (p1 + child_p1, p2.min(child_p2))
            },
        )
    }
    let (p1, p2) = dfs(
        root.as_ref(),
        30_000_000 - (70_000_000 - root.borrow().size),
    );
    println!("Day7 (Rust Rc<RefCell<Node>>): P1: {p1}, P2: {p2}");
}
