#include "args.hpp"
#include "log.hpp"
#include "utils.hpp"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/CommonOptionsParser.h"

#include "parser.hpp"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;

ParserErrorCode generate_reflection_model(const TranslationUnit &unit, ReflectionModel &out_model) {
    std::vector<char*> cmd_args = get_parser_command_args(GLOBAL_CONTROL_FLAGS->cpp_version, GLOBAL_CONTROL_FLAGS->include_dirs, GLOBAL_CONTROL_FLAGS->pre_include_headers, GLOBAL_CONTROL_FLAGS->verbose);
    int argc = cmd_args.size();
    const char** argv = (const char**)cmd_args.data();
    cl::OptionCategory category("parsing");
    auto option_parser = CommonOptionsParser::create(argc, argv, category);

    return ParserErrorCode::Success;
}
