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
    CXErrorCode err = clang_parseTranslationUnit2(
        index, unit.identity_name.c_str(), 
        PARSER_COMMAND_ARGS, 2,
        nullptr, 0,
        CXTranslationUnit_IncludeAttributedTypes | CXTranslationUnit_SingleFileParse,
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
