#pragma once

#include <string>
#include <concepts>
#include "clang/AST/Type.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Type.h"
#include "clang/AST/DeclCXX.h"


namespace zeno::reflect
{
    struct CodeCompilerState {
    };
    
    template <typename T>
    concept ICodeCompiler = requires(T v, CodeCompilerState& state) {
        { v.compile(state) } -> std::convertible_to<std::string>;
    };

    template <typename T, typename TypeToHash>
    concept IHashImpl = requires(T v, const TypeToHash& s) {
        { v(s) } -> std::convertible_to<size_t>;
    };

    class TemplateHeaderGenerator {
        CodeCompilerState m_compiler_state;
        std::stringstream m_rtti_block;

    public:
        std::string compile(CodeCompilerState& state);

        template <ICodeCompiler T>
        void add_rtti_block(T generator) {
            m_rtti_block << generator.compile(m_compiler_state);
        }
    };

    template <typename HashImpl>
    requires IHashImpl<HashImpl, std::string>
    class RTTITypeGenerator {
        std::string m_cpp_type;
        std::string m_name;
        size_t m_hash;

    public:
        RTTITypeGenerator(
            std::string cpp_type,
            std::string name,
            size_t hash
        ) : m_cpp_type(cpp_type), m_name(name), m_hash(hash) {}

        std::string compile(CodeCompilerState& state) {

        }
    };

    class ForwardDeclarationGenerator {
        clang::QualType m_qual_type;

    public:
        ForwardDeclarationGenerator(const clang::QualType& qual_type);

        std::string compile(CodeCompilerState& state);
    };
}
