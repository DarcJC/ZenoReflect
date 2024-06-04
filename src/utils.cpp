#include <string>
#include <cctype>
#include <filesystem>
#include "utils.hpp"
#include "args.hpp"

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
    result.push_back(std::format("-std=c++{}", cpp_version));
    // Set definitions
    // ZENO_REFLECT_PROCESSING will only set when parsing by this generator
    result.push_back("-DZENO_REFLECT_PROCESSING=1");
    result.push_back("-DWITH_REFLECT=1");
    // Add include directories
    for (const std::string& dir : include_dirs) {
        result.push_back(
            std::format("-I{}", dir)
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

std::vector<std::string_view> split(std::string_view str, std::string_view delimiter)
{
    std::vector<std::string_view> parts;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string_view::npos) {
        if (end > start) {
            parts.push_back(str.substr(start, end - start));
        }
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    if (start < str.length()) {
        parts.push_back(str.substr(start));
    }

    return parts;
}

std::string get_file_path_in_header_output(std::string_view filename)
{
    return std::format("{}/{}", GLOBAL_CONTROL_FLAGS->output_dir, filename);
}

std::string relative_path_to_header_output(std::string_view abs_path)
{
    const std::filesystem::path header_output_dir(GLOBAL_CONTROL_FLAGS->output_dir);
    const std::filesystem::path input_path(abs_path);

    return std::filesystem::relative(input_path, header_output_dir).string();
}

void truncate_file(const std::string &path)
{
    std::ofstream s(path, std::ios::out | std::ios::trunc);
    s.close();
}

std::string normalize_filename(std::string_view input)
{
    return std::filesystem::path(input).lexically_normal().filename().string();
}

}
}

