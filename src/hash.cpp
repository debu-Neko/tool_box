#include "tool_box/hash.hpp"

#include <windows.h>
#include <bcrypt.h>

#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace tool_box {

namespace {

constexpr std::size_t kBufferSize = 64 * 1024;  // 64 KiB

LPCWSTR algo_id(HashAlgo algo) {
    switch (algo) {
        case HashAlgo::Md5:    return BCRYPT_MD5_ALGORITHM;
        case HashAlgo::Sha256: return BCRYPT_SHA256_ALGORITHM;
    }
    throw std::invalid_argument("unknown hash algorithm");
}

void check_nt(NTSTATUS status, const char* what) {
    if (status < 0) {
        char buf[128];
        std::snprintf(buf, sizeof(buf), "%s failed: 0x%08lX",
                      what, static_cast<unsigned long>(status));
        throw std::runtime_error(buf);
    }
}

std::string to_hex(const unsigned char* data, std::size_t size) {
    static constexpr char kHex[] = "0123456789abcdef";
    std::string out;
    out.resize(size * 2);
    for (std::size_t i = 0; i < size; ++i) {
        out[2 * i]     = kHex[(data[i] >> 4) & 0x0F];
        out[2 * i + 1] = kHex[data[i] & 0x0F];
    }
    return out;
}

// RAII wrappers so we never leak on exceptions.
struct AlgHandle {
    BCRYPT_ALG_HANDLE h = nullptr;
    ~AlgHandle() { if (h) BCryptCloseAlgorithmProvider(h, 0); }
};

struct HashHandle {
    BCRYPT_HASH_HANDLE h = nullptr;
    ~HashHandle() { if (h) BCryptDestroyHash(h); }
};

}  // namespace

HashAlgo parse_hash_algo(std::string_view name) {
    if (name == "sha256" || name == "SHA256" || name == "Sha256") {
        return HashAlgo::Sha256;
    }
    if (name == "md5" || name == "MD5" || name == "Md5") {
        return HashAlgo::Md5;
    }
    throw std::invalid_argument(
        "unknown algo: " + std::string(name) + " (expected: sha256 or md5)");
}

std::string_view to_string(HashAlgo algo) {
    switch (algo) {
        case HashAlgo::Md5:    return "md5";
        case HashAlgo::Sha256: return "sha256";
    }
    return "unknown";
}

std::string hash_file(const std::filesystem::path& path, HashAlgo algo) {
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("file not found: " + path.string());
    }
    if (!std::filesystem::is_regular_file(path)) {
        throw std::runtime_error("not a regular file: " + path.string());
    }

    AlgHandle alg;
    check_nt(BCryptOpenAlgorithmProvider(&alg.h, algo_id(algo), nullptr, 0),
             "BCryptOpenAlgorithmProvider");

    DWORD obj_size = 0, cb_result = 0;
    check_nt(BCryptGetProperty(alg.h, BCRYPT_OBJECT_LENGTH,
                               reinterpret_cast<PUCHAR>(&obj_size),
                               sizeof(obj_size), &cb_result, 0),
             "BCryptGetProperty(OBJECT_LENGTH)");

    DWORD hash_length = 0;
    check_nt(BCryptGetProperty(alg.h, BCRYPT_HASH_LENGTH,
                               reinterpret_cast<PUCHAR>(&hash_length),
                               sizeof(hash_length), &cb_result, 0),
             "BCryptGetProperty(HASH_LENGTH)");

    std::vector<UCHAR> obj(obj_size);
    HashHandle hash;
    check_nt(BCryptCreateHash(alg.h, &hash.h, obj.data(),
                              static_cast<ULONG>(obj.size()),
                              nullptr, 0, 0),
             "BCryptCreateHash");

    std::ifstream in(path, std::ios::binary);
    if (!in) {
        throw std::runtime_error("cannot open: " + path.string());
    }

    std::vector<char> buffer(kBufferSize);
    while (in) {
        in.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
        const auto got = in.gcount();
        if (got > 0) {
            check_nt(BCryptHashData(hash.h,
                                    reinterpret_cast<PUCHAR>(buffer.data()),
                                    static_cast<ULONG>(got), 0),
                     "BCryptHashData");
        }
    }
    if (!in.eof()) {
        throw std::runtime_error("read error: " + path.string());
    }

    std::vector<UCHAR> digest(hash_length);
    check_nt(BCryptFinishHash(hash.h, digest.data(), hash_length, 0),
             "BCryptFinishHash");

    return to_hex(digest.data(), digest.size());
}

}  // namespace tool_box
