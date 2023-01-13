use std::{
    collections::BTreeMap,
    net::{Ipv4Addr, SocketAddr},
    sync::{Arc, Mutex},
};

use tokio::{
    io::{self, AsyncReadExt, AsyncWriteExt, ErrorKind::InvalidData},
    net::{TcpListener, TcpStream},
};

const MAX_CONNECTIONS: usize = 32;

#[allow(unreachable_code)]
#[tokio::main]
async fn main() -> io::Result<()> {
    let listener = TcpListener::bind((Ipv4Addr::UNSPECIFIED, 1234)).await?;
    println!("Listening on {}...", listener.local_addr()?);

    let connection_count = Arc::new(Mutex::new(0));
    loop {
        let connection @ (_, addr) = match listener.accept().await {
            Ok(connection) => connection,
            Err(e) => {
                eprintln!("Failed to accept connection: {e}");
                continue;
            }
        };
        println!("{addr} connected!");

        {
            let mut connection_count = connection_count.lock().unwrap();
            if *connection_count >= MAX_CONNECTIONS {
                eprintln!("Dropping {addr}: max connections reached");
                continue;
            }
            *connection_count += 1;
        }

        let connection_count = Arc::clone(&connection_count);
        tokio::spawn(async move {
            match handle_connection(connection).await {
                Ok(()) => {}
                Err(e) if e.kind() == io::ErrorKind::UnexpectedEof => {
                    println!("{addr} disconnected");
                }
                Err(e) => {
                    eprintln!("Dropped {addr}: {e}");
                }
            }
            *connection_count.lock().unwrap() -= 1;
        });
    }

    Ok(())
}

async fn handle_connection((mut stream, addr): (TcpStream, SocketAddr)) -> io::Result<()> {
    let mut store = BTreeMap::new();

    loop {
        let mut buf = [0; 9];
        stream.read_exact(&mut buf).await?;

        match buf[0] {
            b'I' => {
                let timestamp = i32::from_be_bytes(buf[1..5].try_into().unwrap());
                let price = i32::from_be_bytes(buf[5..].try_into().unwrap());
                println!("{addr}: insert (timestamp: {timestamp}, price: {price})");

                if store.insert(timestamp, price).is_some() {
                    return Err(InvalidData.into());
                }
            }
            b'Q' => {
                let mintime = i32::from_be_bytes(buf[1..5].try_into().unwrap());
                let maxtime = i32::from_be_bytes(buf[5..].try_into().unwrap());
                print!("{addr}: query (mintime: {mintime}, maxtime: {maxtime})");

                if mintime > maxtime {
                    println!(" => mean price: 0 (mintime > maxtime)");
                    stream.write_i32(0).await?;
                    continue;
                }

                let (price_sum, price_count) = store
                    .range(mintime..=maxtime)
                    .map(|(_, &price)| f64::from(price))
                    .zip(1..)
                    .fold((0.0, 1), |(acc, _), (price, i)| (acc + price, i));
                // Truncation is allowed.
                let mean_price = (price_sum / f64::from(price_count)) as i32;
                println!(" => mean price: {mean_price}");
                stream.write_i32(mean_price).await?;
            }
            _ => return Err(InvalidData.into()),
        }
    }
}
