#include "utils.hpp"

std::ostream &operator<<(std::ostream &stream, const CXString &str)
{
    stream << clang_getCString(str);
    clang_disposeString(str);
    return stream;
}

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
