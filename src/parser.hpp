#pragma once

#include <string>
#include <unordered_map>

enum class TranslationUnitType {
    Header,
    Standalone,
};

enum class ParserErrorCode {
    Success = 0,
    InternalError = 1,
    TUCreationFailure = 2,
};

struct TranslationUnit {
    std::string identity_name;
    std::string source;
    TranslationUnitType type = TranslationUnitType::Standalone;
};

struct ReflectionFieldDecl {
    std::string name;
    uint32_t is_ptr: 1 = false;
    uint32_t is_ref: 1 = false;
};

struct ReflectionFunctionDecl {
    std::string name;
    uint32_t is_pure: 1 = false;
    uint32_t is_virtual: 1 = false;
    uint32_t is_pure_virtual: 1 = false;
};

struct ReflectionStruct {
    std::string name;
    std::unordered_map<std::string, ReflectionFieldDecl> fields;
    std::unordered_map<std::string, ReflectionFunctionDecl> functions;
};

struct ReflectionModel {
    std::unordered_map<std::string, ReflectionStruct> structs;
};

inline static struct ParserState{
} GLOBAL_PARSER_STATE {};

ParserErrorCode generate_reflection_model(const TranslationUnit& unit, ReflectionModel& out_model);
