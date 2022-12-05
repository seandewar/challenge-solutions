const std = @import("std");

extern "c" fn perror(?[*:0]const u8) void;

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();

    var args_it = std.process.args();
    _ = args_it.skip(); // Skip the command used to execute the program.
    while (args_it.next()) |path| {
        const fd = std.c.open(path, std.os.O.RDONLY);
        if (fd == -1) {
            perror("open");
            return error.openFailed;
        }
        defer if (std.c.close(fd) == -1) {
            perror("close");
            std.os.exit(1);
        };

        var buf: [4096]u8 = undefined;
        while (true) {
            const read_len = std.c.read(fd, &buf, buf.len);
            if (read_len == 0) break;
            if (read_len == -1) {
                perror("read");
                return error.readFailed;
            }

            try stdout.writeAll(buf[0..@intCast(usize, read_len)]);
        }
    }
}
