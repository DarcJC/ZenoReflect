#include "args.hpp"
#include "log.hpp"
#include "utils.hpp"
#include "clang/Frontend/CompilerInvocation.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/CommonOptionsParser.h"

#include "parser.hpp"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;

class ReflectionASTConsumer : public ASTConsumer {
public:
    void HandleTranslationUnit(ASTContext &Context) override {
    }
};

class ReflectionGeneratorAction : public ASTFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &compiler, StringRef code) override {
        return std::make_unique<ReflectionASTConsumer>();
    }
};

ParserErrorCode generate_reflection_model(const TranslationUnit &unit, ReflectionModel &out_model) {
    std::unique_ptr<CompilerInvocation> invocation = std::make_unique<CompilerInvocation>();
    std::vector<std::string> args = get_parser_command_args(GLOBAL_CONTROL_FLAGS->cpp_version, GLOBAL_CONTROL_FLAGS->include_dirs, GLOBAL_CONTROL_FLAGS->pre_include_headers, GLOBAL_CONTROL_FLAGS->verbose);

    clang::tooling::runToolOnCodeWithArgs(
        std::make_unique<ReflectionGeneratorAction>(),
        unit.source.c_str(),
        args,
        unit.identity_name.c_str()
    );

    return ParserErrorCode::Success;
}
