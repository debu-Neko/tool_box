#include <catch2/catch_test_macros.hpp>

#include "tool_box/hash.hpp"

#include <filesystem>
#include <fstream>
#include <string>

namespace {

std::filesystem::path write_temp(const std::string& content) {
    auto p = std::filesystem::temp_directory_path() / "tool_box_test_hash.txt";
    std::ofstream out(p, std::ios::binary | std::ios::trunc);
    out.write(content.data(), static_cast<std::streamsize>(content.size()));
    return p;
}

}  // namespace

TEST_CASE("parse_hash_algo accepts known names", "[hash]") {
    using tool_box::HashAlgo;
    using tool_box::parse_hash_algo;

    REQUIRE(parse_hash_algo("sha256") == HashAlgo::Sha256);
    REQUIRE(parse_hash_algo("SHA256") == HashAlgo::Sha256);
    REQUIRE(parse_hash_algo("md5")    == HashAlgo::Md5);
    REQUIRE(parse_hash_algo("MD5")    == HashAlgo::Md5);
}

TEST_CASE("parse_hash_algo rejects unknown names", "[hash]") {
    REQUIRE_THROWS_AS(tool_box::parse_hash_algo("sha1"), std::invalid_argument);
    REQUIRE_THROWS_AS(tool_box::parse_hash_algo(""),     std::invalid_argument);
}

TEST_CASE("sha256 of known content", "[hash]") {
    const auto p = write_temp("hello\n");
    REQUIRE(tool_box::hash_file(p, tool_box::HashAlgo::Sha256)
            == "5891b5b522d5df086d0ff0b110fbd9d21bb4fc7163af34d08286a2e846f6be03");
    std::filesystem::remove(p);
}

TEST_CASE("md5 of known content", "[hash]") {
    const auto p = write_temp("hello\n");
    REQUIRE(tool_box::hash_file(p, tool_box::HashAlgo::Md5)
            == "b1946ac92492d2347c6235b4d2611184");
    std::filesystem::remove(p);
}

TEST_CASE("hash_file throws on missing file", "[hash]") {
    REQUIRE_THROWS_AS(
        tool_box::hash_file("no/such/file.txt", tool_box::HashAlgo::Sha256),
        std::runtime_error);
}
