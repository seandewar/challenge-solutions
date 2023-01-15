use std::{
    borrow::Cow,
    collections::HashSet,
    net::{Ipv4Addr, SocketAddr},
    sync::{Arc, Mutex},
};

use tinyvec::ArrayVec;
use tokio::{
    io::{self, AsyncBufRead, AsyncBufReadExt, AsyncWriteExt, BufReader},
    net::{TcpListener, TcpStream},
    sync::broadcast::{self, error::RecvError},
};

const MAX_NICK_LEN: usize = 16;
type Nick = ArrayVec<[u8; MAX_NICK_LEN]>;
type Nicks = Arc<Mutex<HashSet<Nick>>>;

// Use the single-threaded runtime this time, for no particular reason.
#[tokio::main(flavor = "current_thread")]
async fn main() -> io::Result<()> {
    let listener = TcpListener::bind((Ipv4Addr::UNSPECIFIED, 1234)).await?;
    println!("Listening on {}...", listener.local_addr()?);

    let connected_nicks: Nicks = Arc::new(Mutex::new(HashSet::new()));
    let (msg_tx, _) = broadcast::channel(32);

    loop {
        let (stream, addr) = match listener.accept().await {
            Ok(connection) => connection,
            Err(e) => {
                eprintln!("Failed to accept a connection: {e}");
                continue;
            }
        };
        println!("{addr} connected!");

        let connected_nicks = Arc::clone(&connected_nicks);
        let msg_tx = msg_tx.clone();
        tokio::spawn(async move {
            match handle_connection(BufReader::new(stream), addr, connected_nicks, msg_tx).await {
                Ok(()) => println!("{addr} disconnected"),
                Err(e) if e.kind() == io::ErrorKind::UnexpectedEof => {
                    println!("{addr} disconnected")
                }
                Err(e) => eprintln!("Dropped {addr}: {e}"),
            }
        });
    }
}

// NOTE: the buffer is taken by reference (rather than created inside the function and returned) so
// that the read data isn't lost if the task is cancelled.
async fn read_line<A: tinyvec::Array<Item = u8>, R: AsyncBufRead + Unpin>(
    reader: &mut R,
    buf: &mut ArrayVec<A>,
) -> io::Result<()> {
    loop {
        let fill_buf = reader.fill_buf().await?;
        if fill_buf.is_empty() {
            return Err(io::ErrorKind::UnexpectedEof.into());
        }

        let nl_i = fill_buf.iter().position(|&b| b == b'\n');
        let read_len = nl_i.unwrap_or(fill_buf.len());
        if read_len > buf.capacity() - buf.len() {
            return Err(io::ErrorKind::InvalidData.into());
        }

        buf.extend_from_slice(&fill_buf[..read_len]);
        reader.consume(read_len);
        if nl_i.is_some() {
            reader.consume(1);
            break;
        }
    }
    Ok(())
}

type Message = (Option<Nick>, Arc<[u8]>);

async fn handle_connection(
    mut stream: BufReader<TcpStream>,
    addr: SocketAddr,
    connected_nicks: Nicks,
    msg_tx: broadcast::Sender<Message>,
) -> io::Result<()> {
    stream
        .write_all(b"Welcome to Sean's Budget Chat Tokio Server! Enter your nickname:\n")
        .await?;

    let mut nick = Nick::new();
    read_line(&mut stream, &mut nick).await?;
    if nick.is_empty() || !nick.iter().all(|b| b.is_ascii_alphanumeric()) {
        eprintln!("Dropping {addr}: invalid nick");
        return Err(io::ErrorKind::Other.into());
    }

    let msg = {
        let mut lock = connected_nicks.lock().unwrap();
        if lock.contains(&nick) {
            eprintln!(
                "Dropping {addr}: nick \"{}\" is taken",
                String::from_utf8_lossy(&nick)
            );
            return Err(io::ErrorKind::Other.into());
        };

        println!("{addr}: set nick \"{}\"", String::from_utf8_lossy(&nick));
        let _ = msg_tx.send((
            None,
            Arc::from(
                &(nick.iter().copied())
                    .chain(b" joined".iter().copied())
                    .collect::<ArrayVec<[_; MAX_NICK_LEN + 7]>>()[..],
            ),
        ));

        let msg = if lock.is_empty() {
            Cow::Borrowed(&b"* No connected users\n"[..])
        } else {
            Cow::Owned(
                (b"* Connected users: ".iter().copied())
                    .chain(
                        // intersperse() is not stable yet, so DIY it.
                        lock.iter()
                            .flat_map(|nick| b", ".iter().chain(nick.iter()))
                            .skip(2)
                            .copied(),
                    )
                    .chain(b"\n".iter().copied())
                    .collect(),
            )
        };

        lock.insert(nick);
        msg
    };
    stream.write_all(&msg).await?;

    struct OnDisconnect<'a> {
        connected_nicks: &'a Nicks,
        nick: &'a Nick,
        msg_tx: &'a broadcast::Sender<Message>,
    }

    impl Drop for OnDisconnect<'_> {
        fn drop(&mut self) {
            let _ = self.msg_tx.send((
                None,
                Arc::from(
                    &(self.nick.iter().copied())
                        .chain(b" left".iter().copied())
                        .collect::<ArrayVec<[_; MAX_NICK_LEN + 5]>>()[..],
                ),
            ));
            self.connected_nicks.lock().unwrap().remove(self.nick);
        }
    }

    let _on_disconnect = OnDisconnect {
        connected_nicks: &connected_nicks,
        nick: &nick,
        msg_tx: &msg_tx,
    };

    let mut msg_rx = msg_tx.subscribe();
    let mut msg_buf = ArrayVec::<[u8; 1000]>::new();

    loop {
        tokio::select! {
            result = read_line(&mut stream, &mut msg_buf) => {
                result?;
                println!(
                    "{addr} (nick \"{}\") sends a message: \"{}\"",
                    String::from_utf8_lossy(&nick),
                    String::from_utf8_lossy(&msg_buf),
                );
                msg_tx.send((Some(nick), Arc::from(&msg_buf[..]))).unwrap();
                msg_buf.clear();
            }

            result = msg_rx.recv() => {
                match result {
                    Ok((Some(msg_nick), msg)) if msg_nick != nick => {
                        stream.write_all(
                            &(b"[".iter().copied())
                            .chain(msg_nick.into_iter())
                            .chain(b"] ".iter().copied())
                            .chain(msg.iter().copied())
                            .chain(b"\n".iter().copied())
                            .collect::<ArrayVec<[_; 1000 + MAX_NICK_LEN + 4]>>(),
                        ).await?;
                    }
                    Ok((None, msg)) => {
                        stream.write_all(
                            &(b"* ".iter().copied())
                            .chain(msg.iter().copied())
                            .chain(b"\n".iter().copied())
                            .collect::<ArrayVec<[_; 1000 + 2]>>(),
                        ).await?;
                    }
                    Ok(_) => {}

                    Err(RecvError::Lagged(_)) => {
                        eprintln!("Dropping {addr}: too far behind");
                        return Err(io::ErrorKind::Other.into());
                    }
                    Err(_) => unreachable!(),
                }
            }
        };
    }
}
