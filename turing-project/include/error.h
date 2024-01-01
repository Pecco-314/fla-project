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

std::ostream &operator<<(std::ostream &os, const ArgError::Type &et);

struct CodeError {
    enum class Type;
    Type type;
    Code::Span span;
    std::string expected = "";
    void log(bool verbose) const;
};

enum class CodeError::Type {
    LEXER_INVALID_CHAR,
    PARSER_EXPECTED,
    PARSER_UNEXPECTED_UNDERSCORE,
    PARSER_UNCLOSED_SET,
    PARSER_EMPTY_SET,
    PARSER_EXPECTED_UNDERSCORE_AS_BLANK,
    VALIDATOR_MISSING_UNDERSCORE_IN_TAPE_ALPHABET,
    VALIDATOR_INPUT_ALPHABET_NOT_SUBSET_OF_TAPE_ALPHABET,
    VALIDATOR_INVALID_INITIAL_STATE,
    VALIDATOR_FINAL_STATES_NOT_SUBSET_OF_STATES,
};

std::ostream &operator<<(std::ostream &os, const CodeError::Type &e);

#endif
