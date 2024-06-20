#include <string>
#include <cctype>
#include <filesystem>
#include <cassert>
#include "utils.hpp"
#include "args.hpp"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Expr.h"
#include "clang/AST/PrettyPrinter.h"

namespace zeno {

namespace reflect {

std::optional<std::string> read_file(const std::string &filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return std::nullopt;
    }
    std::stringstream buf;
    buf << file.rdbuf();
    return buf.str();
}

std::vector<std::string> get_parser_command_args(const std::string &cpp_version, std::vector<std::string> &include_dirs, std::vector<std::string> &pre_include_headers, bool verbose)
{
    std::vector<std::string> result;

    // Set Language to c++ (libclang assume a header file which suffix equal to .h is a c source code)
    result.push_back("-x");
    result.push_back("c++");

    // Ignore wanrings
    result.push_back("-Wno-pragma-once-outside-header");

    // Set standrad version
    result.push_back(std::format("-std=c++{}", cpp_version));
    // Set definitions
    // ZENO_REFLECT_PROCESSING will only set when parsing by this generator
    result.push_back("-DZENO_REFLECT_PROCESSING=1");
    result.push_back("-DWITH_REFLECT=1");
    // Add include directories
    for (const std::string& dir : include_dirs) {
        result.push_back(
            std::format("-I{}", dir)
        );
    }
    // Add pre include headers
    for (const std::string& header : pre_include_headers) {
        result.push_back("-include");
        result.push_back(header);
    }

    if (verbose) {
        result.push_back("-v");
    }

    if (verbose) {
        std::cout << "[debug] Arguments passing to Clang:\t\"";
        for (const std::string& str : result) {
            std::cout << str << " ";
        }
        std::cout << "\"" << std::endl;
    }

    return result;
}

bool is_vaild_char(char c)
{
    return c >= -1;
}

std::string trim_start(const std::string &str)
{
    size_t start = 0;
    while (start < str.length() && std::isspace(static_cast<unsigned char>(str[start]))) {
        start++;
    }
    return str.substr(start);
}

std::string trim_end(const std::string &str)
{
    size_t end = str.length();
    while (end > 0 && std::isspace(static_cast<unsigned char>(str[end - 1]))) {
        end--;
    }
    return str.substr(0, end);
}

std::string trim(const std::string &str)
{
    return trim_end(trim_start(str));
}

std::vector<std::string_view> split(std::string_view str, std::string_view delimiter)
{
    std::vector<std::string_view> parts;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string_view::npos) {
        if (end > start) {
            parts.push_back(str.substr(start, end - start));
        }
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    if (start < str.length()) {
        parts.push_back(str.substr(start));
    }

    return parts;
}

std::string get_file_path_in_header_output(std::string_view filename)
{
    return std::format("{}/{}", GLOBAL_CONTROL_FLAGS->output_dir, filename);
}

std::string relative_path_to_header_output(std::string_view abs_path)
{
    const std::filesystem::path header_output_dir(GLOBAL_CONTROL_FLAGS->output_dir);
    const std::filesystem::path input_path(abs_path);

    return std::filesystem::relative(input_path, header_output_dir).string();
}

void truncate_file(const std::string &path)
{
    std::ofstream s(path, std::ios::out | std::ios::trunc);
    s.close();
}

std::string normalize_filename(std::string_view input)
{
    return std::filesystem::path(input).lexically_normal().filename().string();
}

std::string convert_to_valid_cpp_var_name(std::string_view type_name)
{
    std::string varName;
    bool lastWasColon = false;

    for (std::size_t i = 0; i < type_name.size(); ++i) {
        char ch = type_name[i];

        // RecordTypes => struct, class, union
        if (type_name.substr(i, 6) == "struct" && (i + 6 == type_name.size() || type_name[i + 6] == ' ')) {
            varName += "struct_";
            i += 6; // "struct"
        } else if (type_name.substr(i, 5) == "class" && (i + 5 == type_name.size() || type_name[i + 5] == ' ')) {
            varName += "class_";
            i += 5; // "class"
        } else if (type_name.substr(i, 5) == "union" && (i + 5 == type_name.size() || type_name[i + 5] == ' ')) {
            varName += "union_";
            i += 5; // "union"
        } else if (std::isalnum(ch)) { // Valid chars
            varName += ch;
            lastWasColon = false;
        } else if (ch == ':' || ch == ' ') {
            if (!lastWasColon && !varName.empty() && varName.back() != '_') {
                varName += '_';  // Replace ":" or " "
            }
            lastWasColon = (ch == ':');
        }
    }

    if (!varName.empty() && varName.back() == '_') {
        varName.pop_back();
    }

    // Ensure not started with number
    if (!varName.empty() && std::isdigit(varName[0])) {
        varName = "_" + varName;
    }

    return varName;
}

std::string clang_expr_to_string(const clang::Expr *expr)
{
    if (!expr) {
        return "nullptr";
    }
    
    clang::LangOptions lang_opts;
    lang_opts.CPlusPlus = true;
    clang::PrintingPolicy policy(lang_opts);

    std::string str;
    llvm::raw_string_ostream stream(str);
    expr->printPretty(stream, nullptr, policy);

    return str;
}

std::string clang_type_name_no_tag(const clang::QualType& type)
{
    clang::LangOptions lang_opts;
    lang_opts.CPlusPlus = true;
    clang::PrintingPolicy policy(lang_opts);
    policy.SuppressTagKeyword = true;
    return type.getAsString(policy);
}

inja::json parse_param_data(const clang::ParmVarDecl * param_decl)
{

    inja::json param_data;
    clang::QualType type = param_decl->getType();
    param_data["type"] = type.getCanonicalType().getAsString();
    param_data["has_default_arg"] = param_decl->hasDefaultArg();
    if (param_decl->hasDefaultArg()) {
        param_data["default_arg"] = zeno::reflect::clang_expr_to_string(param_decl->getDefaultArg());
    }

    return param_data;

}

inja::json parse_param_data(const clang::FieldDecl *param_decl)
{
    inja::json param_data;
    clang::QualType type = param_decl->getType();
    param_data["type"] = type.getCanonicalType().getAsString();
    param_data["has_default_arg"] = param_decl->hasInClassInitializer();
    if (param_decl->hasInClassInitializer()) {
        param_data["default_arg"] = zeno::reflect::clang_expr_to_string(param_decl->getInClassInitializer());
    }

    return param_data;
}

constexpr uint32_t FNV1aHash::hash_32_fnv1a(std::string_view str) const noexcept
{
    uint32_t hash = internal::FNV1aInternal<uint32_t>::val;
    for (const unsigned char c : str) {
        hash = hash ^ c;
        hash *= internal::FNV1aInternal<uint32_t>::prime;
    }
    return hash;
}

constexpr uint64_t FNV1aHash::hash_64_fnv1a(std::string_view str) const noexcept
{
    uint64_t hash = internal::FNV1aInternal<uint64_t>::val;
    for (const unsigned char c : str) {
        hash = hash ^ c;
        hash *= internal::FNV1aInternal<uint64_t>::prime;
    }
    return hash;
}

size_t FNV1aHash::operator()(std::string_view str) const noexcept
{
    if constexpr (sizeof(size_t) == sizeof(uint32_t)) {
        return hash_32_fnv1a(str);
    } else if constexpr (sizeof(size_t) == sizeof(uint64_t)) {
        return hash_64_fnv1a(str);
    } else {
        assert(false);
    }
}

}
}

