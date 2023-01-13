//! Rather than use std.net for everything, let's manually mess around with
//! std.os sockets, which allows us to use SOCK_NONBLOCK (stage2 doesn't
//! support async yet, and the synchronous implementation is blocking).
//!
//! For fun, let's implement everything using just a single thread.
//! (Ideally, we could look for primes in parallel instead.)
const std = @import("std");
const os = std.os;
const log = std.log;

pub const std_options = struct {
    pub const log_level = switch (@import("builtin").mode) {
        .Debug => .debug,
        .ReleaseSafe, .ReleaseFast, .ReleaseSmall => .info,
    };
};

const Client = struct {
    socket: os.socket_t,
    address: std.net.Ip4Address, // Ip4Address so we can easily format it for logging.
    read_buf: std.BoundedArray(u8, 10 * 1024) = std.BoundedArray(u8, 10 * 1024).init(0) catch unreachable,
    running_prime_test: ?struct { number: u32, max_check: u32, next_check: u32 = 3 } = null,
};
const Clients = [16]?Client;
const PollFds = [1 + @typeInfo(Clients).Array.len]os.pollfd; // Server socket pollfd is index 0.

pub fn main() !void {
    const server_socket = try os.socket(os.AF.INET, os.SOCK.STREAM | os.SOCK.NONBLOCK, os.IPPROTO.TCP);
    defer os.closeSocket(server_socket);

    const listen_address = os.sockaddr.in{
        .port = std.mem.nativeToBig(u16, 1234), // Network byte order.
        .addr = 0,
    };
    try os.bind(server_socket, @ptrCast(*const os.sockaddr, &listen_address), @sizeOf(@TypeOf(listen_address)));
    try os.listen(server_socket, 16);
    log.info("Listening for connections...", .{});

    var clients = [_]?Client{null} ** @typeInfo(Clients).Array.len;
    defer for (clients) |slot| if (slot) |client| os.closeSocket(client.socket);

    var pollfds = [_]os.pollfd{.{ .fd = -1, .events = os.POLL.IN, .revents = undefined }} ** @typeInfo(PollFds).Array.len;
    pollfds[0].fd = server_socket;

    while (true) {
        // Just linearly scan for this.
        const pending_prime_test = blk: for (clients) |slot| {
            if (slot != null and slot.?.running_prime_test != null) break :blk true;
        } else false;
        var ready_clients_count = try os.poll(&pollfds, if (pending_prime_test) 0 else -1);

        if (pollfds[0].revents != 0) {
            ready_clients_count -= 1;
            acceptClient(server_socket, &clients, &pollfds) catch |err| switch (err) {
                error.Overflow => {}, // Error already given.
                error.ProcessFdQuotaExceeded,
                error.SystemFdQuotaExceeded,
                error.SystemResources,
                error.BlockedByFirewall,
                error.ConnectionResetByPeer,
                => log.err("Failed to accept a new client connection: {}", .{err}),
                else => return err,
            };
        }
        if (ready_clients_count == 0 and !pending_prime_test) continue;

        for (pollfds[1..]) |*pollfd, i| {
            const client = clients[i] orelse continue;
            if (pollfd.revents == 0 and client.running_prime_test == null) continue;

            processClient(&clients[i], pollfd) catch |err| switch (err) {
                error.EndOfStream,
                error.OperationAborted,
                error.BrokenPipe,
                error.SystemResources,
                error.ConnectionResetByPeer,
                error.ConnectionTimedOut,
                error.Overflow,
                error.MalformedRequest,
                => continue,
                else => return err,
            };
        }
    }
}

fn acceptClient(server_socket: os.socket_t, clients: *Clients, pollfds: *PollFds) !void {
    var client = Client{ .socket = undefined, .address = undefined };
    client.socket = try os.accept(server_socket, null, null, os.SOCK.NONBLOCK);
    errdefer os.closeSocket(client.socket);

    var addrlen: os.socklen_t = @sizeOf(@TypeOf(client.address.sa));
    try os.getpeername(client.socket, @ptrCast(*os.sockaddr, &client.address.sa), &addrlen);
    log.info("Client {} connected!", .{client.address});

    // Again, just linearly scan (we're lazy).
    const free_slot_i = for (clients) |slot, i| {
        if (slot == null) break i;
    } else {
        log.err("Dropping {}: no client slots available", .{client.address});
        return error.Overflow;
    };
    clients.*[free_slot_i] = client;
    pollfds.*[1 + free_slot_i].fd = client.socket;
}

fn processClient(client_slot: *?Client, pollfd: *os.pollfd) !void {
    const client = &client_slot.*.?;
    errdefer {
        os.closeSocket(client.socket);
        client_slot.* = null;
        pollfd.fd = -1;
    }

    // Make progress on the running prime test, if any.
    if (client.running_prime_test) |*prime_test| prime_test: {
        const max_check = @min(prime_test.max_check + 1, 2000 + prime_test.next_check);

        while (prime_test.next_check < max_check) : (prime_test.next_check += 2) {
            if (prime_test.number % prime_test.next_check != 0) continue;
            // Not prime.
            try sendClientResponse(client.*, false);
            client.running_prime_test = null;
            break :prime_test;
        }
        if (prime_test.next_check > prime_test.max_check) {
            // It's prime!
            try sendClientResponse(client.*, true);
            client.running_prime_test = null;
            break :prime_test;
        }
        return;
    }

    // No prime test is running, try to read some pending data from the socket.
    var read_socket = true;
    const number = blk: while (true) {
        break :blk readClientNumber(client, read_socket) catch |err| switch (err) {
            error.NotPrime => {
                try sendClientResponse(client.*, false);
                read_socket = false; // May have another line buffered in the read_buf.
                continue;
            },
            error.EndOfStream => {
                log.info("Dropping {}: client disconnected", .{client.address});
                return err;
            },
            error.OperationAborted,
            error.BrokenPipe,
            error.ConnectionResetByPeer,
            error.ConnectionTimedOut,
            error.Overflow,
            error.MalformedRequest,
            => {
                log.err("Dropping {}: read error: {}", .{ client.address, err });
                return err;
            },
            else => return err,
        } orelse return;
    };

    client.running_prime_test = .{ .number = number, .max_check = std.math.sqrt(number) };
}

fn sendClientResponse(client: Client, is_prime: bool) !void {
    errdefer |err| log.err("Dropping {}: send error: {}", .{ client.address, err });

    if (is_prime) {
        log.info("Client {}: requested number was prime!", .{client.address});
        _ = try os.send(client.socket, "{\"method\":\"isPrime\",\"prime\":true}\n", 0);
    } else {
        log.info("Client {}: requested number was NOT prime", .{client.address});
        _ = try os.send(client.socket, "{\"method\":\"isPrime\",\"prime\":false}\n", 0);
    }
}

fn readClientNumber(client: *Client, read_socket: bool) !?u32 {
    if (read_socket) blk: {
        const unused_capacity = client.read_buf.unusedCapacitySlice();
        if (unused_capacity.len > 0) {
            const read_len = os.read(client.socket, unused_capacity) catch |err| switch (err) {
                error.WouldBlock => break :blk,
                else => return err,
            };
            if (read_len == 0) return error.EndOfStream;
            client.read_buf.resize(client.read_buf.len + read_len) catch unreachable;
        }
    }

    if (std.mem.indexOfScalar(u8, client.read_buf.slice(), '\n')) |nl_i| {
        const line = client.read_buf.slice()[0..nl_i];
        if (log.defaultLogEnabled(.debug)) {
            // Make the line printable, for debugging purposes.
            var line_copy: @TypeOf(client.read_buf.buffer) = undefined;
            const printable_line = line_copy[0..nl_i];
            for (printable_line) |*print_c, i| {
                const c = client.read_buf.buffer[i];
                print_c.* = if (std.ascii.isPrint(c)) c else '?';
            }
            log.debug("Parsing request from client {} ({} bytes): \"{s}\"", .{ client.address, nl_i, printable_line });
        }

        var parse_buf: [64]u8 = undefined;
        var fba = std.heap.FixedBufferAllocator.init(&parse_buf);
        // std.json.Parser gets stuck and loops forever (Zig is still a young
        // language :P), but std.json.parse() works for our purposes anyway.
        var stream = std.json.TokenStream.init(line);
        const message = std.json.parse(
            struct {
                method: []const u8,
                number: union(enum) {
                    string: []const u8,
                    unsigned_int: u32,
                    float_or_negative: f32,
                },
            },
            &stream,
            .{ .allocator = fba.allocator(), .ignore_unknown_fields = true },
        ) catch return error.MalformedRequest;
        if (!std.mem.eql(u8, "isPrime", message.method)) return error.MalformedRequest;

        // A circular buffer would be nicer, but this buffer is small anyway.
        client.read_buf.replaceRange(0, nl_i + 1, &.{}) catch unreachable;

        switch (message.number) {
            .string => return error.MalformedRequest,
            .unsigned_int => |number| {
                log.info("Client {}: requests number {}", .{ client.address, number });
                return if (number == 2 or (number > 2 and number % 2 != 0)) number else error.NotPrime;
            },
            .float_or_negative => |number| {
                log.info("Client {}: requests number {}", .{ client.address, number });
                return error.NotPrime;
            },
        }
    } else if (client.read_buf.len == client.read_buf.buffer.len) {
        return error.Overflow;
    }

    return null;
}
