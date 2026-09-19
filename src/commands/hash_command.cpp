#include "tool_box/commands/hash_command.hpp"
#include "tool_box/hash.hpp"

#include <CLI/CLI.hpp>
#include <fmt/core.h>
#include <nlohmann/json.hpp>

#include <stdexcept>
#include <string>

namespace tool_box {

void register_hash_command(CLI::App& app) {
    auto* cmd = app.add_subcommand("hash", "Compute the hash of a file");

    auto file = std::make_shared<std::string>();
    auto algo = std::make_shared<std::string>("sha256");
    auto json = std::make_shared<bool>(false);

    cmd->add_option("file", *file, "Path to the file")->required();
    cmd->add_option("-a,--algo", *algo, "Hash algorithm: sha256 or md5")
        ->default_val("sha256");
    cmd->add_flag("--json", *json, "Output as JSON");

    cmd->callback([file, algo, json]() {
        HashAlgo a = parse_hash_algo(*algo);
        const std::string digest = hash_file(*file, a);

        if (*json) {
            nlohmann::json out = {
                {"file", *file},
                {"algo", std::string(to_string(a))},
                {"hash", digest},
            };
            fmt::print("{}\n", out.dump());
        } else {
            fmt::print("{}  {}\n", digest, *file);
        }
    });
}

}  // namespace tool_box
