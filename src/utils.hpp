#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <ostream>
#include <iostream>
#include <optional>
#include <vector>
#include "clang-c/Index.h"
#include "fmt/format.h"

std::ostream& operator<<(std::ostream& stream, const CXString& str);

std::optional<std::string> read_file(const std::string& filepath);

// inline const char* PARSER_COMMAND_ARGS[] = {"-x", "c++", "-std=c++17", "-DZENO_REFLECT_PROCESSING=1", "-DWITH_REFLECT=1"};

inline static std::vector<char*>& get_parser_command_args(
    const std::string& cpp_version
) {
    static std::vector<char*> PARSER_COMMAND_ARGS;
    auto convert = [] (const std::string& s) -> char* {
        char* pc = new char[s.size() + 1];
        std::strcpy(pc, s.c_str());
        pc[s.size()] = '\0';
        return pc;
    };

    if (PARSER_COMMAND_ARGS.empty()) {
        PARSER_COMMAND_ARGS.push_back(strdup("-x"));
        PARSER_COMMAND_ARGS.push_back(strdup("c++"));
        PARSER_COMMAND_ARGS.push_back(convert(fmt::format("-std=c++{}", cpp_version)));
        PARSER_COMMAND_ARGS.push_back(strdup("-DZENO_REFLECT_PROCESSING=1"));
        PARSER_COMMAND_ARGS.push_back(strdup("-DWITH_REFLECT=1"));
    }

    return PARSER_COMMAND_ARGS;
}

