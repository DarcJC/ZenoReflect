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

inline static std::vector<char*>& get_parser_command_args(
    const std::string& cpp_version,
    std::vector<std::string>& include_dirs,
    std::vector<std::string>& pre_include_headers,
    bool verbose = false
) {
    static std::vector<char*> PARSER_COMMAND_ARGS;
    auto convert = [] (const std::string& s) -> char* {
        char* pc = new char[s.size() + 1];
        std::strcpy(pc, s.c_str());
        pc[s.size()] = '\0';
        return pc;
    };

    if (PARSER_COMMAND_ARGS.empty()) {
        // Set Language to c++ (libclang assume a header file which suffix equal to .h is a c source code)
        PARSER_COMMAND_ARGS.push_back(strdup("-x"));
        PARSER_COMMAND_ARGS.push_back(strdup("c++"));
        // Set standrad version
        PARSER_COMMAND_ARGS.push_back(convert(fmt::format("-std=c++{}", cpp_version)));
        // Set definitions
        // ZENO_REFLECT_PROCESSING will only set when parsing by this generator
        PARSER_COMMAND_ARGS.push_back(strdup("-DZENO_REFLECT_PROCESSING=1"));
        PARSER_COMMAND_ARGS.push_back(strdup("-DWITH_REFLECT=1"));
        // Add include directories
        for (const std::string& dir : include_dirs) {
            PARSER_COMMAND_ARGS.push_back(convert(
                fmt::format("-I{}", dir)
            ));
        }
        // Add pre include headers
        for (const std::string& header : pre_include_headers) {
            PARSER_COMMAND_ARGS.push_back(strdup("-include"));
            PARSER_COMMAND_ARGS.push_back(convert(
                header
            ));
        }
    }

    if (verbose) {
        std::cout << "libclang args:\t\"";
        for (const char* str : PARSER_COMMAND_ARGS) {
            std::cout << str << " ";
        }
        std::cout << "\"" << std::endl;
    }

    return PARSER_COMMAND_ARGS;
}

