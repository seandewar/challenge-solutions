use std::{
    fs::File,
    io::{BufRead, BufReader},
};

fn main() {
    let file = File::open("input").unwrap();
    let mut reader = BufReader::new(file);
    let mut line_buf = String::new();

    let (mut p1, mut p2) = (0, 0);
    loop {
        line_buf.clear();
        if reader.read_line(&mut line_buf).unwrap() == 0 {
            break;
        }

        let line = line_buf.trim_start().strip_prefix("Game").unwrap();
        let (id, line) = line.trim_start().split_once(':').unwrap();
        let id: u32 = id.parse().unwrap();

        let mut impossible = false;
        let (mut max_r, mut max_g, mut max_b) = (0, 0, 0);

        for subset in line.split(';').map(str::trim) {
            let (mut r, mut g, mut b) = (12, 13, 14);

            for mut action in subset.split(',').map(str::split_whitespace) {
                let quantity: u32 = action.next().unwrap().parse().unwrap();
                let colour = action.next().unwrap();
                assert!(action.next().is_none());

                let (rem, max): (&mut u32, &mut u32) = match colour {
                    "red" => (&mut r, &mut max_r),
                    "green" => (&mut g, &mut max_g),
                    "blue" => (&mut b, &mut max_b),
                    _ => unreachable!(),
                };

                *rem = rem.checked_sub(quantity).unwrap_or_else(|| {
                    impossible = true;
                    0
                });
                *max = quantity.max(*max);
            }
        }

        if !impossible {
            p1 += id;
        }
        p2 += max_r * max_g * max_b;
    }

    println!("Day2: P1: {p1}, P2: {p2}");
}
