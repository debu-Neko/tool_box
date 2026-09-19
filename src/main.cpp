#include <CLI/CLI.hpp>
#include <fmt/core.h>
#include <string>

int main(int argc, char** argv) {
    CLI::App app{"tool_box - a small Windows toolbox"};
    app.set_version_flag("-v,--version", "0.1.0");

    std::string name = "world";
    app.add_option("-n,--name", name, "Name to greet");

    CLI11_PARSE(app, argc, argv);

    fmt::print("Hello, {}!\n", name);
    return 0;
}
