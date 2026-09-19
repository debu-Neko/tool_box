#pragma once

#include <filesystem>
#include <string>
#include <string_view>

namespace tool_box {

enum class HashAlgo {
    Md5,
    Sha256,
};

// Parse "sha256" / "md5" (case-insensitive). Throws std::invalid_argument.
HashAlgo parse_hash_algo(std::string_view name);

// "sha256" / "md5" for output.
std::string_view to_string(HashAlgo algo);

// Compute the digest of a file, streaming in 64 KiB chunks.
// Returns lowercase hex. Throws std::runtime_error on IO errors.
std::string hash_file(const std::filesystem::path& path, HashAlgo algo);

}  // namespace tool_box
