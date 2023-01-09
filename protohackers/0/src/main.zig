//! Async would be better, but stage2 doesn't really support it yet.
//! This solution does not allocate.
const std = @import("std");
const log = std.log;
const StreamServer = std.net.StreamServer;
const Thread = std.Thread;

const Client = struct {
    connection: StreamServer.Connection,
    thread: Thread,

    fn handleClient(client: *?Client) !void {
        const address = client.*.?.connection.address;
        defer {
            log.info("{} disconnected", .{address});
            client.*.?.connection.stream.close();
            client.* = null;
        }

        var read_buf: [2048]u8 = undefined;
        const reader = client.*.?.connection.stream.reader();
        const writer = client.*.?.connection.stream.writer();
        while (true) {
            const read_len = try reader.read(&read_buf);
            if (read_len == 0) return;
            const slice = read_buf[0..read_len];
            try writer.writeAll(slice);

            // Printify it so we can see what's going on.
            for (slice) |*c| {
                if (!std.ascii.isPrint(c.*)) c.* = '?';
            }
            log.debug("{} sends {} bytes: \"{s}\"", .{ address, read_len, slice });
        }
    }
};

pub fn main() !void {
    var server = StreamServer.init(.{});
    defer server.deinit();

    try server.listen(std.net.Address.initIp4(.{0} ** 4, 1234));
    log.info("Listening for connections...", .{});

    var clients = [_]?Client{null} ** 64;
    defer for (clients) |*client_opt| {
        const client = client_opt orelse continue;
        client.connection.stream.close();
        client.thread.join();
    };

    while (true) acceptClient(&server, &clients) catch continue;
}

fn acceptClient(server: *StreamServer, clients: []?Client) !void {
    var connection = server.accept() catch |err| {
        log.err("Failed to accept a connection: {}", .{err});
        return err;
    };
    errdefer connection.stream.close();
    log.info("{} connected!", .{connection.address});

    var free_i: usize = 0; // Just linearly scan for a free client slot.
    while (free_i < clients.len and clients[free_i] != null) : (free_i += 1) {}
    if (free_i == clients.len) {
        log.err("Dropping {}: no client slots available", .{connection.address});
        return error.TooManyClients;
    }

    const client = &clients[free_i];
    client.* = .{ .connection = connection, .thread = undefined };
    errdefer client.* = null;
    client.*.?.thread = Thread.spawn(.{}, Client.handleClient, .{client}) catch |err| {
        log.err("Dropping {}: could not spawn thread: {}", .{ connection.address, err });
        return err;
    };
}
