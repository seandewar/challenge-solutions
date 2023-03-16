pub fn LinkedList(comptime T: type) type {
    return struct {
        pub const Node = struct {
            prev: ?*Node = null,
            next: ?*Node = null,
            data: T,
        };

        first: ?*Node = null,
        last: ?*Node = null,
        len: usize = 0,

        pub fn push(self: *@This(), node: *Node) void {
            if (self.last) |last| {
                last.next = node;
                node.prev = last;
            } else {
                self.first = node;
            }
            self.last = node;
            self.len += 1;
        }

        pub fn pop(self: *@This()) ?*Node {
            const last = self.last orelse return null;
            if (last.prev) |prev| {
                prev.next = null;
            } else {
                self.first = null;
            }
            self.last = last.prev;
            self.len -= 1;
            return last;
        }

        pub fn shift(self: *@This()) ?*Node {
            const first = self.first orelse return null;
            if (first.next) |next| {
                next.prev = null;
            } else {
                self.last = null;
            }
            self.first = first.next;
            self.len -= 1;
            return first;
        }

        pub fn unshift(self: *@This(), node: *Node) void {
            if (self.first) |first| {
                first.prev = node;
                node.next = first;
            } else {
                self.last = node;
            }
            self.first = node;
            self.len += 1;
        }

        pub fn delete(self: *@This(), node: *Node) void {
            var it = self.first;
            while (it) |n| : (it = n.next) if (n == node) {
                if (n.prev) |prev| prev.next = n.next;
                if (n.next) |next| next.prev = n.prev;
                if (n == self.first) self.first = n.next;
                if (n == self.last) self.last = n.prev;
                self.len -= 1;
                return;
            };
        }
    };
}
