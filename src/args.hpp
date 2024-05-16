#pragma once

#include "argparse.hpp"

struct ControlFlags : public argparse::Args {
    std::string& input_source_path = arg("filepath", "Translation unit path to read");
    std::string& cpp_version = kwarg("stdc++", "Set cpp standard (default: 17)").set_default("17");
};


ControlFlags parse_args(int argc, char** argv);
