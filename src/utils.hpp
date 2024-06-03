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

}
}
