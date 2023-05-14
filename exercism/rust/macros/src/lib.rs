#[macro_export]
macro_rules! hashmap {
    () => { ::std::collections::HashMap::new() };
    ($($k:expr => $v:expr),+ $(,)?) => {{
        let cap = 0 $(+ $crate::hashmap!(@cap $k))*;
        let mut hm = ::std::collections::HashMap::with_capacity(cap);
        $(hm.insert($k, $v);)*
        hm
    }};
    (@cap $_:tt) => { 1 };
}
