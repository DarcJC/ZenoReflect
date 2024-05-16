#include <string>
#include <fstream>
#include <sstream>
#include <ostream>
#include <iostream>
#include <optional>
#include "clang-c/Index.h"

enum class TranslationUnitType {
    Header,
    Standalone,
};

struct TranslationUnit {
    std::string identity_name;
    std::string source;
    TranslationUnitType type = TranslationUnitType::Standalone;
};

std::ostream& operator<<(std::ostream& stream, const CXString& str) {
    stream << clang_getCString(str);
    clang_disposeString(str);
    return stream;
}

std::optional<std::string> read_file(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return std::nullopt;
    }
    std::stringstream buf;
    buf << file.rdbuf();
    return buf.str();
}

const char* PARSER_COMMAND_ARGS[] = {"-DZENO_REFLECT_PROCESSING=1\0", "-DWITH_REFLECT=1\0"};

int traverse_file_ast(const TranslationUnit& unit) {
    CXIndex index = clang_createIndex(0, 1);
    CXTranslationUnit tu = nullptr;
    CXErrorCode err = clang_parseTranslationUnit2(
        index, unit.identity_name.c_str(), 
        PARSER_COMMAND_ARGS, 2,
        nullptr, 0,
        CXTranslationUnit_None,
        &tu
    );

    if (!tu) {
        std::cerr << "Failed to create translation unit from source" << std::endl;
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


int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: <Source File Path>" << std::endl;
        return 1;
    }

    std::string filepath(argv[1]);
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
