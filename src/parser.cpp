#include <fstream>
#include "args.hpp"
#include "log.hpp"
#include "utils.hpp"
#include "parser.hpp"
#include "serialize.hpp"
#include "codegen.hpp"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

std::string get_file_path_in_header_output(std::string_view filename) {
    return std::format("{}/{}", GLOBAL_CONTROL_FLAGS->output_dir, filename);
}

void truncate_file(const std::string& path) {
    std::ofstream s(path, std::ios::out | std::ios::trunc);
    s.close();
}


class ReflectionGeneratorAction : public ASTFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &compiler, StringRef code) override {
        return std::make_unique<ReflectionASTConsumer>();
    }
};

ParserErrorCode generate_reflection_model(const TranslationUnit &unit, ReflectionModel &out_model) {
    out_model.debug_name = unit.identity_name;
    std::vector<std::string> args = zeno::reflect::get_parser_command_args(GLOBAL_CONTROL_FLAGS->cpp_version, GLOBAL_CONTROL_FLAGS->include_dirs, GLOBAL_CONTROL_FLAGS->pre_include_headers, GLOBAL_CONTROL_FLAGS->verbose);

    const std::string gen_template_header_path = get_file_path_in_header_output("generated_templates.hpp");
    truncate_file(gen_template_header_path);

    if (!clang::tooling::runToolOnCodeWithArgs(
        std::make_unique<ReflectionGeneratorAction>(),
        unit.source.c_str(),
        args,
        unit.identity_name.c_str()
    )) {
        return ParserErrorCode::InternalError;
    }

    return ParserErrorCode::Success;
}

TypeAliasMatchCallback::TypeAliasMatchCallback(ReflectionASTConsumer *context) : m_context(context) {}

void TypeAliasMatchCallback::run(const MatchFinder::MatchResult &result)
{
    const TypeDecl* decll = nullptr;
    QualType underlying_type;
    if (const TypedefDecl* decl = result.Nodes.getNodeAs<TypedefDecl>(ASTLabels::TYPEDEF_LABEL)) {
        underlying_type = decl->getUnderlyingType();
        decll = decl;
    } else if (const TypeAliasDecl* decl = result.Nodes.getNodeAs<TypeAliasDecl>(ASTLabels::TYPE_ALIAS_LABEL)) {
        underlying_type = decl->getUnderlyingType();
        decll = decl;
    }

    if (decll && m_context) {
        zeno::reflect::RTTITypeGenerator rtti(underlying_type);
        m_context->template_header_generator.add_rtti_block(rtti);
    }
}

RecordTypeMatchCallback::RecordTypeMatchCallback(ReflectionASTConsumer *context) : m_context(context) {}

void RecordTypeMatchCallback::run(const MatchFinder::MatchResult &result)
{
    if (const CXXRecordDecl* record_decl = result.Nodes.getNodeAs<CXXRecordDecl>(ASTLabels::RECORD_LABEL)) {
        if (!record_decl->hasDefinition()) {
            if (GLOBAL_CONTROL_FLAGS->verbose) {
                llvm::outs() << "[debug] «" << record_decl->getNameAsString() << "» is only a declaration, skipped.\n";
            }
            return;
        }

        if (record_decl->getNumBases() > 0) {
            for (const auto& base : record_decl->bases()) {
            }
        }

        if (record_decl->hasAttrs()) {
            for (const auto* attr : record_decl->attrs()) {
                if (const AnnotateAttr *Annotate = dyn_cast<AnnotateAttr>(attr)) {
                    MetadataContainer container = MetadataParser::parse(Annotate->getAnnotation().str());
                }
            }
        }
    }
}

void ReflectionASTConsumer::HandleTranslationUnit(ASTContext &context)
{
    scoped_context = &context;

    const std::string gen_template_header_path = get_file_path_in_header_output("generated_templates.hpp");
    truncate_file(gen_template_header_path);

    MatchFinder type_alias_finder{};
    DeclarationMatcher typealias_matcher = typeAliasDecl().bind(ASTLabels::TYPE_ALIAS_LABEL);
    DeclarationMatcher typedef_matcher = typedefDecl().bind(ASTLabels::TYPEDEF_LABEL);
    type_alias_finder.addMatcher(typealias_matcher, type_alias_handler.get());
    type_alias_finder.addMatcher(typedef_matcher, type_alias_handler.get());
    type_alias_finder.matchAST(context);

    DeclarationMatcher record_type_matcher = cxxRecordDecl().bind(ASTLabels::RECORD_LABEL);
    MatchFinder record_finder{};
    record_finder.addMatcher(record_type_matcher, record_type_handler.get());
    record_finder.matchAST(context);

    // generate header
    const std::string generated_templates = template_header_generator.compile();
    
    std::ofstream generated_templates_stream(gen_template_header_path, std::ios::out | std::ios::trunc);
    generated_templates_stream << generated_templates;

    scoped_context = nullptr;
}

void ReflectionASTConsumer::add_type_mapping(const std::string &alias_name, QualType real_name)
{
    if (GLOBAL_CONTROL_FLAGS->verbose) {
        llvm::outs() << "[debug] " << "Added type alias: «" << alias_name << "» → «" << real_name << "»\n";
    }
    type_name_mapping.insert_or_assign(alias_name, real_name);
}
