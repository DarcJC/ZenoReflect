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

