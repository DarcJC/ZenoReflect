#pragma once

#include "argparse.hpp"

struct ControlFlags : public argparse::Args {
    std::string& input_source_path = arg("filepath", "Translation unit path to read");
    std::string& cpp_version = kwarg("stdc++", "Set cpp standard (default: 17)").set_default("17");
    bool& verbose = flag("v,verbose", "Print extra information");
    std::vector<std::string>& include_dirs = kwarg("I,include_dirs", "Include directories").multi_argument().set_default(std::vector<std::string>{});
    std::vector<std::string>& pre_include_headers = kwarg("H,pre_include_header", "Automatic place those headers in all sources").multi_argument().set_default(std::vector<std::string>{});
};

ControlFlags parse_args(int argc, char** argv);

static inline ControlFlags* GLOBAL_CONTROL_FLAGS = nullptr;
