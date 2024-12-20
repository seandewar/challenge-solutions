use std::{
    collections::VecDeque,
    fs::File,
    io::{BufRead, BufReader},
};

fn main() {
    const MIN_CHEAT_TIME_SAVE: u16 = 100;
    const DIRS: [(i16, i16); 4] = [(1, 0), (0, 1), (-1, 0), (0, -1)];

    #[derive(Copy, Clone, Eq, PartialEq)]
    enum Square {
        Unreachable,
        Reachable(u16),
        Wall,
    }

    let (mut grid, width, height, start, end) = {
        let reader = BufReader::new(File::open("input").unwrap());
        let mut lines = reader.lines().peekable();

        let mut grid: Vec<Square> = Vec::new();
        let width: i16 = lines
            .peek()
            .unwrap()
            .as_ref()
            .unwrap()
            .len()
            .try_into()
            .unwrap();
        let mut y: i16 = 0;
        let mut start: Option<(i16, i16)> = None;
        let mut end: Option<(i16, i16)> = None;

        for mut line in lines.map(|s| s.unwrap().into_bytes()) {
            assert!(i16::try_from(line.len()).unwrap() == width);
            if let Some(i) = line.iter().position(|&c| c == b'S') {
                assert!(start.is_none() && !line.iter().skip(i + 1).any(|&c| c == b'S'));
                start = Some((i.try_into().unwrap(), y));
            }
            if let Some(i) = line.iter().position(|&c| c == b'E') {
                assert!(end.is_none() && !line.iter().skip(i + 1).any(|&c| c == b'E'));
                end = Some((i.try_into().unwrap(), y));
                line[i] = b'.';
            }

            grid.extend(line.drain(..).map(|c| match c {
                b'S' => Square::Reachable(0),
                b'.' => Square::Unreachable,
                b'#' => Square::Wall,
                _ => unreachable!(),
            }));
            y += 1;
        }

        (grid, width, y, start.unwrap(), end.unwrap())
    };

    let grid_index = |(x, y): (i16, i16)| {
        (x >= 0 && y >= 0 && x < width && y < height)
            .then(|| usize::try_from(y * width + x).unwrap())
    };

    {
        let mut q = VecDeque::from([start]);
        for t in 0.. {
            for _ in 0..q.len() {
                let (x, y) = q.pop_front().unwrap();
                for (dx, dy) in DIRS {
                    let next_pos = (x + dx, y + dy);
                    let Some(next_i) = grid_index(next_pos) else {
                        continue;
                    };
                    if grid[next_i] != Square::Unreachable {
                        continue;
                    }

                    grid[next_i] = Square::Reachable(t + 1);
                    if next_pos != end {
                        q.push_back(next_pos);
                    }
                }
            }

            if q.is_empty() {
                break;
            }
        }

        assert!(grid.iter().all(|&s| s != Square::Unreachable));
    }

    let mut p1: u32 = 0;
    let mut p2: u32 = 0;

    for y in 0..height {
        for x in 0..width {
            let pos = (x, y);
            let i = grid_index(pos).unwrap();
            let Square::Reachable(t0) = grid[i] else {
                continue;
            };

            let count_cheats = |max_t: u16| {
                let mut count = 0;

                let d = i16::try_from(max_t).unwrap();
                for y2 in y - d..=y + d {
                    for x2 in x - d..=x + d {
                        let cheat_t = ((y2 - y).abs() + (x2 - x).abs()).try_into().unwrap();
                        if !(1..=max_t).contains(&cheat_t) {
                            continue;
                        }

                        let Some(j) = grid_index((x2, y2)) else {
                            continue;
                        };
                        let Square::Reachable(t1) = grid[j] else {
                            continue;
                        };

                        if t1.saturating_sub(t0 + cheat_t) >= MIN_CHEAT_TIME_SAVE {
                            count += 1;
                        }
                    }
                }

                count
            };

            p1 += count_cheats(2);
            p2 += count_cheats(20);
        }
    }

    println!("Day20: P1: {p1}, P2: {p2}");
}
