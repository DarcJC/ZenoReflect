#include <string>
#include <cctype>
#include "utils.hpp"

namespace zeno {

namespace reflect {

std::optional<std::string> read_file(const std::string &filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return std::nullopt;
    }
    std::stringstream buf;
    buf << file.rdbuf();
    return buf.str();
}

std::vector<std::string> get_parser_command_args(const std::string &cpp_version, std::vector<std::string> &include_dirs, std::vector<std::string> &pre_include_headers, bool verbose)
{
    std::vector<std::string> result;

    // Set Language to c++ (libclang assume a header file which suffix equal to .h is a c source code)
    result.push_back("-x");
    result.push_back("c++");

    // Ignore wanrings
    result.push_back("-Wno-pragma-once-outside-header");

    // Set standrad version
    result.push_back(fmt::format("-std=c++{}", cpp_version));
    // Set definitions
    // ZENO_REFLECT_PROCESSING will only set when parsing by this generator
    result.push_back("-DZENO_REFLECT_PROCESSING=1");
    result.push_back("-DWITH_REFLECT=1");
    // Add include directories
    for (const std::string& dir : include_dirs) {
        result.push_back(
            fmt::format("-I{}", dir)
        );
    }
    // Add pre include headers
    for (const std::string& header : pre_include_headers) {
        result.push_back("-include");
        result.push_back(header);
    }

    if (verbose) {
        result.push_back("-v");
    }

    if (verbose) {
        std::cout << "[debug] Arguments passing to Clang:\t\"";
        for (const std::string& str : result) {
            std::cout << str << " ";
        }
        std::cout << "\"" << std::endl;
    }

    return result;
}

std::string trim_start(const std::string &str)
{
    size_t start = 0;
    while (start < str.length() && std::isspace(str[start])) {
        start++;
    }
    return str.substr(start);
}

std::string trim_end(const std::string &str)
{
    size_t end = str.length();
    while (end > 0 && std::isspace(str[end - 1])) {
        end--;
    }
    return str.substr(0, end);
}

std::string trim(const std::string &str)
{
    return trim_end(trim_start(str));
}

}
}

