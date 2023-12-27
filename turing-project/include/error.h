#ifndef __ERROR_H__
#define __ERROR_H__

#include "code.h"

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
    Code::Span span;
    std::string expected = "";
    void log(bool verbose) const;
};

enum class CodeError::Type {
    LEXER_INVALID_CHAR,
    PARSER_EXPECTED_TEXT,
    PARSER_EXPECTED_ID,
    PARSER_EXPECTED_INTEGER,
    PARSER_EXPECTED_VALID_CHAR,
    PARSER_UNEXPECTED_UNDERSCORE,
    PARSER_UNCLOSED_SET,
    PARSER_EMPTY_SET,
    PARSER_EXPECTED_UNDERSCORE_AS_BLANK,
    VALIDATOR_MISSING_UNDERSCORE_IN_G,
    VALIDATOR_S_NOT_SUBSET_OF_G,
    VALIDATOR_INVALID_INITIAL_STATE,
    VALIDATOR_F_NOT_SUBSET_OF_Q,
    VALIDATOR_INVALID_NUM_TAPES,
};

#endif
