// basic zig hello world

const std = @import("std");

// pub fn main() !void {
//     const stdout = std.fs.File.stdout();
//     try stdout.writeAll("Hello, World\n");
//
// }
//
pub fn main() !void {
    std.debug.print("hello world\n", .{});
    std.debug.print("hello {s}\n", .{"world"});
    const stdout = std.fs.File.stdout();
    try stdout.writeAll("Hello, World\n");
    try std.fs.File.stdout().writeAll("Hello, World\n");
}
