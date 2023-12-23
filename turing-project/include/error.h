#ifndef __ERROR_H__
#define __ERROR_H__

#include "code.h"
#include <memory>

struct ArgError {
    enum class Type;
    Type type;
    std::string arg;
    void log() const;
};

enum class ArgError::Type {
    INVALID_OPTION,
    TOO_MANY_ARGS,
    TOO_FEW_ARGS,
};

struct CodeError {
    enum class Type;
    Type type;
    std::shared_ptr<Code> code;
    Code::Cursor st;
    Code::Cursor ed;
    void log(bool verbose) const;
};

enum class CodeError::Type {
    LEXER_INVALID_CHAR,
    PARSER_EXPECTED_Q,
    PARSER_EXPECTED_EQUAL,
    PARSER_EXPECTED_LBRACE, 
    PARSER_EXPECTED_ID,
    PARSER_UNCLOSED_BRACE,
};

#endif
