const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const disasm_exe = b.addExecutable(.{
        .name = "z8086-disasm",
        .root_source_file = .{ .path = "src/disasm.zig" },
        .target = target,
        .optimize = optimize,
    });
    b.installArtifact(disasm_exe);

    const sim_exe = b.addExecutable(.{
        .name = "z8086-sim",
        .root_source_file = .{ .path = "src/sim.zig" },
        .target = target,
        .optimize = optimize,
    });
    b.installArtifact(sim_exe);

    const run_cmd = b.addRunArtifact(disasm_exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);

    const unit_tests = b.addTest(.{
        .root_source_file = .{ .path = "src/test.zig" },
        .target = target,
        .optimize = optimize,
    });
    const run_unit_tests = b.addRunArtifact(unit_tests);

    const test_step = b.step("test", "Run unit tests");
    test_step.dependOn(&run_unit_tests.step);
}
