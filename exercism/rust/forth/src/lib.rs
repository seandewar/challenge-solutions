use std::{collections::HashMap, fmt::Write, result};

pub type Value = i32;
pub type Result = result::Result<(), Error>;

#[derive(Default)]
pub struct Forth {
    stack: Vec<Value>,
    words: HashMap<String, Vec<String>>,
}

#[derive(Debug, PartialEq, Eq)]
pub enum Error {
    DivisionByZero,
    StackUnderflow,
    UnknownWord,
    InvalidWord,
}

impl Forth {
    pub fn new() -> Self {
        Self::default()
    }

    pub fn stack(&self) -> &[Value] {
        &self.stack
    }

    fn get_user_word<'w>(&self, word: &'w str) -> Option<(&str, &'w str, usize)> {
        let (word, version): (&str, Option<usize>) = word
            .split_once("__") // May be used to disambiguate different versions of a word.
            .map(|(w, i)| (w, Some(i.parse().unwrap())))
            .unwrap_or((word, None));

        let exprs = self.words.get(word)?;
        let version = version.unwrap_or(exprs.len() - 1);
        exprs.get(version).map(|e| (e.as_str(), word, version))
    }

    pub fn eval(&mut self, input: &str) -> Result {
        let mut it = input.split_whitespace();
        while let Some(tok) = it.next() {
            if tok.chars().all(|c| c.is_ascii_digit()) {
                self.stack.push(tok.parse().unwrap());
                continue;
            }

            let tok = tok.to_ascii_lowercase();
            if let Some((expr, _, _)) = self.get_user_word(&tok) {
                // Not unnecessary, as eval() exclusively borrows self, it *could* (even though it
                // doesn't) potentially change the stored word expr returned by get_user_word(),
                // which uses a shared reference on the expr, so the borrow checker complains.
                #[allow(clippy::unnecessary_to_owned)]
                self.eval(&expr.to_owned())?;
                continue;
            }

            let pop = |s: &mut Self| s.stack.pop().ok_or(Error::StackUnderflow);
            let len = self.stack.len();
            match tok.as_str() {
                "dup" => {
                    self.stack
                        .push(self.stack.last().copied().ok_or(Error::StackUnderflow)?);
                }
                "drop" => {
                    self.stack.pop().ok_or(Error::StackUnderflow)?;
                }
                "swap" | "over" if len < 2 => return Err(Error::StackUnderflow),
                "swap" => self.stack.swap(len - 1, len - 2),
                "over" => self.stack.push(self.stack[len - 2]),

                "+" | "-" | "*" | "/" => {
                    let (b, a) = (pop(self)?, pop(self)?);
                    self.stack.push(match tok.as_str() {
                        "+" => a + b,
                        "-" => a - b,
                        "*" => a * b,
                        "/" if b != 0 => a / b,
                        _ => return Err(Error::DivisionByZero),
                    });
                }

                ":" => {
                    let word = it
                        .next()
                        .filter(|w| w.chars().any(|c| !c.is_ascii_digit() && !w.contains("__")))
                        .map(|w| w.to_ascii_lowercase())
                        .ok_or(Error::InvalidWord)?;

                    let mut expr = String::new();
                    while let Some(tok) =
                        Some(it.next().ok_or(Error::InvalidWord)?).filter(|&t| t != ";")
                    {
                        if let Some((_, word, version)) = self.get_user_word(tok) {
                            write!(&mut expr, "{word}__{version} ").unwrap();
                        } else {
                            write!(&mut expr, "{tok} ").unwrap();
                        }
                    }
                    self.words.entry(word).or_default().push(expr);
                }

                _ => return Err(Error::UnknownWord),
            }
        }

        Ok(())
    }
}
