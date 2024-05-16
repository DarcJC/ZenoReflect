#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <ostream>
#include <iostream>
#include <optional>
#include "clang-c/Index.h"
#include "fmt/format.h"

std::ostream& operator<<(std::ostream& stream, const CXString& str);

std::optional<std::string> read_file(const std::string& filepath);

inline const char* PARSER_COMMAND_ARGS[] = {"-x", "c++", "-std=c++17", "-DZENO_REFLECT_PROCESSING=1", "-DWITH_REFLECT=1"};

