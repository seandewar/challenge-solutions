use std::fs;

fn main() {
    let input = fs::read("src/input").unwrap();
    let width: isize = input
        .iter()
        .position(|&c| c == b'\n')
        .map_or(input.len(), |i| i + 1)
        .try_into()
        .unwrap();
    let height = (isize::try_from(input.len()).unwrap() + (width - 1)) / width;

    let char_at = |x: isize, y: isize| {
        (x >= 0 && y >= 0 && x < width && y < height)
            .then(|| usize::try_from(y * width + x).unwrap())
            .filter(|&i| i < input.len())
            .map(|i| input[i])
    };

    let (mut p1, mut p2) = (0usize, 0usize);
    for y in 0..height {
        for x in 0..width {
            match char_at(x, y) {
                Some(b'X') => {
                    p1 += [
                        (1, 0),
                        (-1, 0),
                        (0, 1),
                        (0, -1),
                        (1, 1),
                        (-1, 1),
                        (1, -1),
                        (-1, -1),
                    ]
                    .into_iter()
                    .filter(|(dx, dy)| {
                        b"MAS".iter().zip(1..4).rev().all(|(&c, i)| {
                            char_at(x + i * dx, y + i * dy).is_some_and(|c2| c == c2)
                        })
                    })
                    .count()
                }

                Some(b'A') => {
                    if [(-1, -1), (-1, 1)].into_iter().all(|(dx, dy)| {
                        matches!(
                            (char_at(x + dx, y + dy), char_at(x - dx, y - dy)),
                            (Some(b'M'), Some(b'S')) | (Some(b'S'), Some(b'M'))
                        )
                    }) {
                        p2 += 1;
                    }
                }

                _ => {}
            };
        }
    }

    println!("Day4: P1: {p1}, P2: {p2}");
}
