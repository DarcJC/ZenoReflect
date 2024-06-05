#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <ostream>
#include <iostream>
#include <optional>
#include <vector>
#include <format>
#include <string_view>

namespace zeno {

namespace reflect {

std::optional<std::string> read_file(const std::string& filepath);

std::vector<std::string> get_parser_command_args(
    const std::string& cpp_version,
    std::vector<std::string>& include_dirs,
    std::vector<std::string>& pre_include_headers,
    bool verbose = false
);

std::string trim_start(const std::string& str);
std::string trim_end(const std::string& str);
std::string trim(const std::string& str);

std::vector<std::string_view> split(std::string_view str, std::string_view delimiter);

std::string get_file_path_in_header_output(std::string_view filename);
std::string relative_path_to_header_output(std::string_view abs_path);
void truncate_file(const std::string& path);

std::string normalize_filename(std::string_view input);

struct FNV1aHash {
    
    template <typename T>
    struct FNV1aInternal {
        static constexpr uint32_t val = 0x811c9dc5U;
        static constexpr uint32_t prime = 0x1000193U;
    };

    template <>
    struct FNV1aInternal<uint64_t> {
        static constexpr uint64_t val = 0xcbf29ce484222325ULL;
        static constexpr uint64_t prime = 0x100000001b3ULL;
    };

    constexpr uint32_t hash_32_fnv1a(std::string_view str) const noexcept;
    constexpr uint64_t hash_64_fnv1a(std::string_view str) const noexcept;

    size_t operator()(std::string_view str) const noexcept;
};

}
}
