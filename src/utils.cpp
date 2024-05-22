#include "utils.hpp"

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
