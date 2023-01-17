use std::{
    collections::HashMap,
    io,
    net::{Ipv4Addr, UdpSocket},
};

fn main() -> io::Result<()> {
    let socket = UdpSocket::bind((Ipv4Addr::UNSPECIFIED, 1234))?;
    println!("Socket bound to {}...", socket.local_addr().unwrap());

    let mut store = HashMap::with_capacity(64);
    store.insert(b"version".to_vec(), b"seano".to_vec());

    let mut buf = [0; 1000];
    loop {
        let (recv_len, from_addr) = match socket.recv_from(&mut buf) {
            Ok(v) => v,
            Err(e) => {
                eprintln!("recv error: {e}");
                continue;
            }
        };
        println!(
            "{from_addr}: \"{}\"",
            String::from_utf8_lossy(&buf[..recv_len])
        );

        match buf[..recv_len]
            .iter()
            .position(|&b| b == b'=')
            .map(|i| buf[..recv_len].split_at(i))
            .map(|(k, v)| (k, &v[1..]))
        {
            Some((b"version", _)) => continue,
            Some((k, v)) => {
                store.insert(k.to_vec(), v.to_vec());
            }
            None => {
                if let Some(v) = store.get(&buf[..recv_len]) {
                    buf[recv_len] = b'=';
                    buf[recv_len + 1..recv_len + 1 + v.len()].copy_from_slice(v);

                    if let Err(e) = socket.send_to(&buf[..recv_len + 1 + v.len()], from_addr) {
                        eprintln!("send error ({from_addr}): {e}");
                    }
                }
            }
        }
    }
}
