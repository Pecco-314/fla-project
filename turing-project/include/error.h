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
    std::string info = "";
    void log(bool verbose) const;
};

enum class CodeError::Type {
    LEXER_INVALID_CHAR,
    PARSER_EXPECTED,
    PARSER_UNEXPECTED_UNDERSCORE,
    PARSER_UNCLOSED_SET,
    PARSER_TRANSITION_TOO_FEW_ITEMS,
    PARSER_TRANSITION_NOT_ON_SAME_LINE,
    PARSER_EMPTY_SET,
    VALIDATOR_MISSING_UNDERSCORE_IN_TAPE_ALPHABET,
    VALIDATOR_INPUT_ALPHABET_NOT_SUBSET_OF_TAPE_ALPHABET,
    VALIDATOR_INVALID_STATE,
    VALIDATOR_FINAL_STATES_NOT_SUBSET_OF_STATES,
    VALIDATOR_TRASITION_ITEM_INVALID_LENGTH,
    VALIDATOR_NOT_IN_TAPE_SYMBOL_AND_NOT_WILDCARD,
    VALIDATOR_INVALID_DIRECTION,
};

std::ostream &operator<<(std::ostream &os, const CodeError::Type &e);

#endif
