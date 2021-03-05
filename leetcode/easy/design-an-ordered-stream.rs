// https://leetcode.com/problems/design-an-ordered-stream
//
// Complexity: runtime new() O(1), insert() O(i), space O(i). [where i is the max id_key]
//
// Note that the average String value lengths don't factor into the complexities here, as they are
// allocated by the caller, and are only moved by us.

#[derive(Default)]
struct OrderedStream {
    buf: Vec<Option<String>>,
    idx: usize,
}

impl OrderedStream {
    fn new(n: i32) -> Self {
        Default::default()
    }

    fn insert(&mut self, id_key: i32, value: String) -> Vec<String> {
        self.buf.resize(self.buf.len().max(id_key as _), None);
        self.buf[id_key as usize - 1] = Some(value);

        let chunk: Vec<_> = self.buf[self.idx..]
            .iter()
            .take_while(|x| x.is_some())
            .map(|x| x.clone().unwrap())
            .collect();
        self.idx += chunk.len();
        chunk
    }
}
