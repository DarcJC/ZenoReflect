#include <limits>
#include <sstream>
#include <iostream>
#include "metadata.hpp"
#include "args.hpp"

MetadataType string_to_metadata_type(const std::string &str)
{
    if (str == "struct") {
        return MetadataType::Struct;
    } else if (str == "enum") {
        return MetadataType::Enum;
    } else if (str == "function") {
        return MetadataType::Function;
    } else if (str == "enum_value") {
        return MetadataType::EnumValue;
    } else if (str == "field") {
        return MetadataType::StructField;
    } else if (str == "param") {
        return MetadataType::FunctionParameter;
    }
    return MetadataType::None;
}

MetadataContainer parse_metadata_dsl(const std::string &in_dsl)
{
    return MetadataParser::parse(in_dsl);
}

MetadataContainer MetadataParser::parse(const std::string &in_dsl)
{
    auto parser = MetadataParser(in_dsl);
    return parser.run();
}

MetadataParser::MetadataParser(std::string in_dsl)
    : current_text(std::move(in_dsl))
    , m_end(current_text.size())
{
    m_state.is_slate = 0;
    m_state.inside_quote = 0;
}

MetadataParser::Token MetadataParser::next_token()
{
    std::stringstream word;
    while (true) {
        if (m_pos >= m_end) {
            return Token {
                .type = TokenType::EndOfFile,
                .start_range = m_pos,
                .end_range = m_pos,
            };
        }
        
        const char& c = current_text.at(m_pos);
        char next_char = '\0';
        if (current_text.size() > m_pos + 1) {
            next_char = current_text.at(m_pos + 1);
        }
        size_t length = 1;

        switch (c) {
            case '(':
                if (!m_state.inside_quote) {
                    return Token {
                        .type = TokenType::LeftBracket,
                        .start_range = m_pos,
                        .end_range = ++m_pos,
                    };
                }
            case ')':
                if (!m_state.inside_quote) {
                    return Token {
                        .type = TokenType::RightBracket,
                        .start_range = m_pos,
                        .end_range = ++m_pos,
                    };
                }
            case '=':
                if (!m_state.inside_quote) {
                    return Token {
                        .type = TokenType::Equal,
                        .start_range = m_pos,
                        .end_range = ++m_pos,
                    };
                }
            case ',':
                if (!m_state.inside_quote) {
                    return Token {
                        .type = TokenType::Comma,
                        .start_range = m_pos,
                        .end_range = ++m_pos,
                    };
                }
            case '\\':
                switch (next_char) {
                    case '\\':
                        word << "\\";
                        break;
                    case 'n':
                        word << "\n";
                        break;
                    case 't':
                        word << "\t";
                        break;
                    case '"':
                        word << "\"";
                        break;
                    case '(':
                        word << "(";
                        break;
                    case ')':
                        word << ')';
                        break;
                    case '=':
                        word << '=';
                        break;
                    case ',':
                        word << ',';
                        break;
                    default:
                        if (GLOBAL_CONTROL_FLAGS->verbose) {
                            std::cout << "[debug] Unknown escape character \"" << "\\" << next_char << "\"";
                        }
                }
                m_pos += 2;
                length += 2;
                break;
            case '\"':
                if (m_state.inside_quote) {
                    m_state.inside_quote = false;
                    m_pos++;
                    return Token {
                        .type = TokenType::Word,
                        .start_range = m_pos - length,
                        .end_range = m_pos,
                        .word_value = std::make_optional<std::string>(word.str()),
                    };
                } else {
                    m_state.inside_quote = true;
                    m_pos++;
                    length++;
                }
                break;
            default:
                if (is_operator(next_char)) {
                    m_pos ++;
                    return Token {
                        .type = TokenType::Word,
                        .start_range = m_pos - length,
                        .end_range = m_pos,
                        .word_value = std::make_optional<std::string>(word.str()),
                    };
                } else {
                    m_pos++;
                    length++;
                }
        }
    }

    return Token {
        .type = TokenType::Unknown,
        .start_range = std::numeric_limits<size_t>::infinity(),
        .end_range = std::numeric_limits<size_t>::infinity(),
    };
}

bool MetadataParser::is_operator(char c)
{
    return c == '(' || c == ')' || c == '=' || c == ',';
}

MetadataContainer MetadataParser::run()
{
    MetadataContainer container{};

    do {
        current_token = std::make_optional(next_token());
    } while (current_token->type != TokenType::EndOfFile) {
    }

    return container;
}
