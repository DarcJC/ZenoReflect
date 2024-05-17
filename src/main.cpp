#include "args.hpp"
#include "utils.hpp"

enum class TranslationUnitType {
    Header,
    Standalone,
};

struct TranslationUnit {
    std::string identity_name;
    std::string source;
    TranslationUnitType type = TranslationUnitType::Standalone;
};

int traverse_file_ast(const TranslationUnit& unit) {
    CXIndex index = clang_createIndex(0, 1);
    CXTranslationUnit tu = nullptr;
    std::vector<char*> cmd_args = get_parser_command_args(GLOBAL_CONTROL_FLAGS->cpp_version, GLOBAL_CONTROL_FLAGS->include_dirs, GLOBAL_CONTROL_FLAGS->pre_include_headers, GLOBAL_CONTROL_FLAGS->verbose);
    CXErrorCode err = clang_parseTranslationUnit2(
        index, unit.identity_name.c_str(), 
        cmd_args.data(), cmd_args.size(),
        nullptr, 0,
        CXTranslationUnit_IncludeAttributedTypes,
        &tu
    );

    if (!tu) {
        std::cerr << "Failed to create translation unit from source: " << err << std::endl;
        return 3;
    }

    CXCursor root = clang_getTranslationUnitCursor(tu);
    clang_visitChildren(
        root,
        [] (CXCursor cursor, CXCursor parent, CXClientData client_data) {
            std::cout << "Cursor kind: " << clang_getCursorKindSpelling(clang_getCursorKind(cursor)) << std::endl;
            if (clang_getCursorKind(cursor) == CXCursorKind::CXCursor_AnnotateAttr) {
                std::cout << clang_getCursorSpelling(cursor) << std::endl;
            }

            return CXChildVisit_Recurse;
        },
        nullptr
    );

    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(index);

    return 0;
}

int main(int argc, char* argv[]) {
    ControlFlags flags = parse_args(argc, argv);
    GLOBAL_CONTROL_FLAGS = &flags;
    if (flags.verbose) {
        flags.print();
    }

    std::string filepath = flags.input_source_path;
    std::optional<std::string> source_str = read_file(filepath);
    if (!source_str.has_value()) {
        std::cerr << "Source is empty" << std::endl;
        return 2;
    }
    std::string source = source_str.value();

    return traverse_file_ast({
        .identity_name = filepath,
        .source = source,
        .type = TranslationUnitType::Header,
    });
}
