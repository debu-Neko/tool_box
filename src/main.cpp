#include <CLI/CLI.hpp>

#include "tool_box/commands/hash_command.hpp"

int main(int argc, char** argv) {
    CLI::App app{"tool_box - a small Windows toolbox"};
    app.set_version_flag("-v,--version", "0.1.0");
    app.require_subcommand(1);

    tool_box::register_hash_command(app);

    CLI11_PARSE(app, argc, argv);
    return 0;
}
